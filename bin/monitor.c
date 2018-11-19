/*
Cuckoo Sandbox - Automated Malware Analysis.
Copyright (C) 2010-2015 Cuckoo Foundation.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <windows.h>
#include "config.h"
#include "diffing.h"
#include "hooking.h"
#include "ignore.h"
#include "log.h"
#include "memory.h"
#include "misc.h"
#include "monitor.h"
#include "native.h"
#include "pipe.h"
#include "sleep.h"
#include "symbol.h"
#include "unhook.h"
#include "SPUtilstd.h"



char* iret(){
}

static uint32_t _parse_mode(const char *mode)
{
    uint32_t ret = HOOK_MODE_ALL;
    while (*mode != 0)
    {
        if (*mode == ' ' || *mode == ',')
        {
            mode++;
            continue;
        }

        if (*mode >= '0' && *mode <= '9')
        {
            ret = strtoul(mode, NULL, 10);
            break;
        }

        if (strnicmp(mode, "dumptls", 7) == 0)
        {
            ret |= 0; //HOOK_MODE_DUMPTLS; //modified by simpower91 这里没一点鸟用，反而造成程序崩溃
            mode += 7;
            continue;
        }

        if (strnicmp(mode, "iexplore", 8) == 0)
        {
            ret |= HOOK_MODE_BROWSER;
            mode += 8;
            continue;
        }

        if (strnicmp(mode, "office", 6) == 0)
        {
            ret |= HOOK_MODE_OFFICE | HOOK_MODE_EXPLOIT;
            mode += 6;
            continue;
        }

        if (strnicmp(mode, "pdf", 3) == 0)
        {
            ret |= HOOK_MODE_PDF | HOOK_MODE_EXPLOIT;
            mode += 3;
            continue;
        }

        if (strnicmp(mode, "exploit", 7) == 0)
        {
            ret |= HOOK_MODE_EXPLOIT;
            mode += 7;
            continue;
        }

        // Report.. find a more proper way? At this point the pipe has not
        // yet been initialized, so.
        message_box(NULL, "Invalid Monitor Mode", mode, 0);
    }
    return ret;
}
char *GetExePath(void)
{
    char szFilePath[MAX_PATH + 1] = {0};
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    (strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串
    char *path = szFilePath;

    return path;
}

void config_read1(config_t *cfg)
{
    char buf[512], config_fname[MAX_PATH];
    char *cwd;
    //cwd = _getcwd(buf, 512);
    //cwd = GetExePath();
    cwd=getlocal("analyst.cfg");
    //strcat(cwd, "\\analyst.cfg");
    sprintf(config_fname, cwd, GetCurrentProcessId());

    memset(cfg, 0, sizeof(config_t));

    FILE *fp = fopen(config_fname, "rb");

    if (fp == NULL)
    {
        message_box(NULL, "Error fetching configuration file! This is a "
                          "serious error. If encountered, please notify the Cuckoo "
                          "Developers as this error prevents analysis.",
                    "Cuckoo Error", 0);
        return;
    }

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        // Cut off the newline.
        char *p = strchr(buf, '\r');
        if (p != NULL)
            *p = 0;

        p = strchr(buf, '\n');
        if (p != NULL)
            *p = 0;

        // Split key=value.
        p = strchr(buf, '=');
        if (p == NULL)
            continue;

        *p = 0;

        const char *key = buf, *value = p + 1;

        if (strcmp(key, "pipe") == 0)
        {
            strncpy(cfg->pipe_name, value, sizeof(cfg->pipe_name));
        }
        else if (strcmp(key, "logpipe") == 0)
        {
            strncpy(cfg->logpipe, value, sizeof(cfg->logpipe));
        }
        else if (strcmp(key, "shutdown-mutex") == 0)
        {
            strncpy(cfg->shutdown_mutex, value, sizeof(cfg->shutdown_mutex));
        }
        else if (strcmp(key, "first-process") == 0)
        {
            cfg->first_process = value[0] == '1';
        }
        else if (strcmp(key, "startup-time") == 0)
        {
            cfg->startup_time = strtoul(value, NULL, 10);
        }
        else if (strcmp(key, "force-sleepskip") == 0)
        {
            cfg->force_sleep_skip = value[0] == '1';
        }
        else if (strcmp(key, "hashes-path") == 0)
        {
            strncpy(cfg->hashes_path, value, sizeof(cfg->hashes_path));
        }
        else if (strcmp(key, "diffing-enable") == 0)
        {
            cfg->diffing_enable = value[0] == '1';
        }
        else if (strcmp(key, "track") == 0)
        {
            cfg->track = value[0] == '1';
        }
        else if (strcmp(key, "mode") == 0)
        {
            cfg->mode = _parse_mode(value);
        }
        else if (strcmp(key, "disguise") == 0)
        {
            cfg->disguise = value[0] == '1';
        }
        else if (strcmp(key, "pipe-pid") == 0)
        {
            cfg->pipe_pid = value[0] == '1';
        }
        else if (strcmp(key, "trigger") == 0)
        {
            strncpy(cfg->trigger, value, sizeof(cfg->trigger));
        }
    }
    fclose(fp);
    //DeleteFile(config_fname);
}

void monitor_init(HMODULE module_handle)
{
    // Sends crashes to the process rather than showing error popup boxes etc.
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT |
        SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

    config_t cfg;
    //沙箱版的改为 config_read(&cfg);安天剑为config_read1(&cfg);
    //config_read1(&cfg);
    config_read(&cfg);

    // Required to be initialized before any logging starts.
    mem_init();

    // Initialize capstone without our custom allocator as it is
    // not available yet.
    hook_init(module_handle);

    pipe_init(cfg.pipe_name, cfg.pipe_pid);
    native_init();

    // Re-initialize capstone with our custom allocator which is now
    // accessible after native_init().
    hook_init2();

    misc_init(cfg.shutdown_mutex);
    diffing_init(cfg.hashes_path, cfg.diffing_enable);

    copy_init();
    log_init(cfg.logpipe, cfg.track);
    ignore_init();

    misc_init2(&monitor_hook, &monitor_unhook);

    sleep_init(cfg.first_process, cfg.force_sleep_skip, cfg.startup_time);

    // Disable the unhook detection for now. TODO Re-enable.
    // unhook_init_detection(cfg.first_process);

    //comment by Simpower91 这个使得loadlibrary返回0，lasterror 1114
    //hide_module_from_peb(module_handle);  

    if(cfg.disguise != 0) {
        // Set the processor count to two.
        set_processor_count(2);

        // Pretend like we have two extra gigabytes of memory.
        add_virtual_memory(2 * 1024 * 1024 * 1024ull);
    }

    symbol_init(module_handle);

    // Should be the last as some of the other initialization routines extract
    // the image size, EAT pointers, etc while the PE header is still intact.
    //comment by Simpower91 这个使得getprocaddress返回0，lasterror 127
    //destroy_pe_header(module_handle);

    misc_set_monitor_options(cfg.track, cfg.mode, cfg.trigger);
    return;
}

void monitor_hook(const char *library, void *module_handle)
{
    // Initialize data about each hook.
    for (hook_t *h = sig_hooks(); h->funcname != NULL; h++) {
        // If a specific library has been specified then we skip all other
        // libraries. This feature is used in the special hook for LdrLoadDll.
        if(library != NULL && stricmp(h->library, library) != 0) {
            continue;
        }

        // We only hook this function if the monitor mode is "hook everything"
        // or if the monitor mode matches the mode of this hook.
        if(g_monitor_mode != HOOK_MODE_ALL &&
                (g_monitor_mode & h->mode) == 0) {
            continue;
        }

        // Return value 1 indicates to retry the hook. This is important for
        // delay-loaded function forwarders as the delay-loaded DLL may
        // already have been loaded. In that case we want to hook the function
        // forwarder right away. (Note that the library member of the hook
        // object is updated in the case of retrying).

        //comment by simpower91, cause i use inlineHookKing for hooking API
        //while (hook(h, module_handle) == 1);
        *h->orig = (FARPROC) iret;
    }
}

void monitor_unhook(const char *library, void *module_handle)
{
    (void) library;

    for (hook_t *h = sig_hooks(); h->funcname != NULL; h++) {
        // This module was unloaded.
        if(h->module_handle == module_handle) {
            h->is_hooked = 0;
            h->addr = NULL;
        }

        // This is a hooked function which doesn't belong to a particular DLL.
        // Therefore the module handle is a nullptr and we simply check
        // whether the address of the original function is still in-memory.
        if(h->module_handle == NULL && range_is_readable(h->addr, 16) == 0) {
            h->is_hooked = 0;
            h->addr = NULL;
        }
    }
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
    (void) hModule; (void) lpReserved;


    if(dwReason == DLL_PROCESS_ATTACH && is_ignored_process() == 0) {
        monitor_init(hModule);
        monitor_hook(NULL, NULL);
        pipe("LOADED:%d,%d", get_current_process_id(), g_monitor_track);
        //MessageBoxA(0,"load","a",0);
    }

    return TRUE;
}
