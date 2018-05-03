#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include "assembly.h"
#include "capstone.h"
#include "x86.h"
#include "hooking.h"
#include "memory.h"
#include "misc.h"
#include "native.h"
#include "ntapi.h"
#include "log.h"
#include "pipe.h"
#include "symbol.h"
#include "unhook.h"

#define MISSING_HANDLE_COUNT 128
#define FUNCTIONSTUBSIZE 128

#define DEBUG 1

static SYSTEM_INFO g_si;
static csh g_capstone;

static slab_t g_function_stubs;

uintptr_t g_monitor_start;
uintptr_t g_monitor_end;

static uintptr_t g_ntdll_start;
static uintptr_t g_ntdll_end;

static uint32_t g_missing_handle_count;
static HMODULE g_missing_handles[MISSING_HANDLE_COUNT];

static const char *g_missing_blacklist[] = {
    NULL,
};

// Return address for Old_LdrLoadDll. Will be used later on to decide whether
// we are "inside" the monitor.
static uintptr_t g_Old_LdrLoadDll_address;

static void *_cs_malloc(size_t size)
{
    return mem_alloc(size);
}

static void *_cs_calloc(size_t nmemb, size_t size)
{
    return mem_alloc(nmemb * size);
}

static void *_cs_realloc(void *ptr, size_t size)
{
    return mem_realloc(ptr, size);
}

static void _cs_free(void *ptr)
{
    mem_free(ptr);
}

static void _capstone_init()
{
#if __x86_64__
    cs_open(CS_ARCH_X86, CS_MODE_64, &g_capstone);
#else
    cs_open(CS_ARCH_X86, CS_MODE_32, &g_capstone);
#endif
}


int hook1(hook_t *h, void *module_handle)
{
    if(h->is_hooked != 0) {
        return 0;
    }

    h->module_handle = module_handle;
    if(h->module_handle == NULL) {
        h->module_handle = GetModuleHandle(h->library);

        // There is only one case in which a nullptr module handle is
        // allowed and that's when there is an address callback and the
        // library starts as well as ends with two underscores.
        const char *end = h->library + strlen(h->library);
        if(h->module_handle == NULL &&
                (h->library[0] == '_' && h->library[1] == '_' &&
                end[-1] == '_' && end[-2] == '_') == 0) {
            return 0;
        }
    }

    // If an address callback has been provided, try to locate the functions'
    // address through it.
    if(h->addr == NULL && h->addrcb != NULL) {
        uint32_t module_size =
            module_image_size((const uint8_t *) h->module_handle);

        h->addr = h->addrcb(h, (uint8_t *) h->module_handle, module_size);

        if(h->addr == NULL) {
            if((h->report & HOOK_PRUNE_RESOLVERR) != HOOK_PRUNE_RESOLVERR) {
                pipe("DEBUG:Error resolving function %z!%z through our "
                    "custom callback.", h->library, h->funcname);
            }
            return -1;
        }
    }

    // Try to obtain the address dynamically.
    if(h->addr == NULL) {
        h->addr = (uint8_t *) GetProcAddress(h->module_handle, h->funcname);
        if(h->addr == NULL) {
            if((h->report & HOOK_PRUNE_RESOLVERR) != HOOK_PRUNE_RESOLVERR) {
                pipe("DEBUG:Error resolving function %z!%z.",
                    h->library, h->funcname);
            }
            return -1;
        }
    }

    if(h->type == HOOK_TYPE_NORMAL && hook_determine_start(h) < 0) {
        pipe("CRITICAL:Error determining start of function %z!%z.",
            h->library, h->funcname);
        return -1;
    }

    // Handle delay loaded forwarders. In some situations an exported symbol
    // will forward execution to another DLL. If this other DLL is delay
    // loaded then we can only hook the function after the delay-loaded DLL
    // has been loaded. In addition to that we'll want to hook the function
    // in the delay-loaded DLL rather than in the current DLL. So we update
    // the library of this hook to represent the one of the delay-loaded DLL.
    IMAGE_DELAYLOAD_DESCRIPTOR *did = NULL;

#if __x86_64__
    // In 64-bit mode delay-loaded function stubs start with a "lea eax, addr"
    // instruction followed by a relative jump.
    if(memcmp(h->addr, "\x48\x8d\x05", 3) == 0 &&
            (h->addr[7] == 0xeb || h->addr[7] == 0xe9)) {
        uint8_t *target = asm_get_rel_jump_target(&h->addr[7]);

        // We're now going to look for the delay import descriptor structure.
        for (uint32_t idx = 0; idx < 128; idx++, target++) {
            // We're looking for a "lea ecx, addr" instruction. Not using
            // capstone here as it seems to have difficulties disassembling
            // various xmm related instructions.
            if(memcmp(target, "\x48\x8d\x0d", 3) == 0) {
                target += *(int32_t *)(target + 3) + 7;
                did = (IMAGE_DELAYLOAD_DESCRIPTOR *) target;
                break;
            }
        }
    }
#else
    // In 32-bit mode delay-loaded function stubs start with a "mov eax, addr"
    // instruction followed by a relative jump.
    if(*h->addr == 0xb8 && (h->addr[5] == 0xeb || h->addr[5] == 0xe9)) {
        uint8_t *target = asm_get_rel_jump_target(&h->addr[5]);

        // We're now going to look for the delay import descriptor structure.
        for (uint32_t idx = 0; idx < 32; idx++) {
            // We're looking for a "push addr" instruction.
            if(*target == 0x68) {
                did = *(IMAGE_DELAYLOAD_DESCRIPTOR **)(target + 1);
                break;
            }
            target += lde(target);
        }
    }
#endif

    // We identified this function to be a forwarder for a delay-loaded DLL
    // function. Update the library, set the earlier located address to a
    // null pointer (so it'll be resolved again), and return success.
    if(did != NULL) {
        // We cheat a little bit here but that should be fine.
        char *library = slab_getmem(&g_function_stubs);
        library_from_asciiz((const char *) h->module_handle + did->DllNameRVA,
            library, slab_size(&g_function_stubs));

        h->library = library;
        h->module_handle = GetModuleHandle(library);
        h->addr = NULL;

        // We're having a special case here. When we return 1, the monitor
        // will attempt to re-apply the hook (but this time against the new
        // library). So we should only do this if the new module is already
        // in-memory.
        return h->module_handle != NULL ? 1 : 0;
    }

    h->func_stub = slab_getmem(&g_function_stubs);
    memset(h->func_stub, 0xcc, slab_size(&g_function_stubs));

    if(h->orig != NULL) {
        *h->orig = (FARPROC) h->func_stub;
    }

    if(h->type == HOOK_TYPE_NORMAL) {
        // Create the original function stub.
        h->stub_used = hook_create_stub(h->func_stub,
            h->addr, ASM_JUMP_32BIT_SIZE + h->skip);
    }
    else if(h->type == HOOK_TYPE_INSN) {
        h->stub_used = hook_insn(h, h->insn_signature);
    }
    else if(h->type == HOOK_TYPE_GUARD) {
        if(hook_hotpatch_guardpage(h) < 0) {
            return -1;
        }
    }

    if(h->stub_used < 0) {
        pipe(
            "CRITICAL:Error creating function stub for %z!%z.",
            h->library, h->funcname
        );
        return -1;
    }

    uint8_t region_original[FUNCTIONSTUBSIZE];
    memcpy(region_original, h->addr, h->stub_used);

    // Patch the original function.
    if(hook_create_jump(h) < 0) {
        return -1;
    }

    unhook_detect_add_region(h->funcname, h->addr, region_original,
        h->addr, h->stub_used);

    if(h->initcb != NULL) {
        h->initcb(h);
    }

    h->is_hooked = 1;
    return 0;
}

    int g_var = 0;
    void print_line(char *str)
    {
        if (str != NULL)
            printf("%s\r \n", str);
        else
            printf("null string\r\n");
    }

    
    int main (int argc, char **argv)
    {
        int l_var = 1;
        print_line(" error hello world!");
        printf("g_var = %d, l_var = %d.\r\n", g_var, l_var);
        hook(NULL,NULL);
        //error("g_var = %d, l_var = %d.\r\n", g_var, l_var);
        return 0;
    }
