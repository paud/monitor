memcpy
======

Signature::

    * Calling convention: __cdecl
    * Category: memcpy
    * Library: msvcrt
    * Is Success: ret != NULL
    * Return value: void *

Parameters::

    * void *dest 
    * const void *source
    * size_t n

Logging::

    i dest_addr dest
    i source_addr source
    i count n

    
CreateProcessInternalW
=======================

Signature::

    * Calling convention: WINAPI
    * Library: KernelBase
    * Logging: always
    * Mode: exploit
    * Return value: BOOL
    * Special: true

Parameters::

    *  LPVOID lpUnknown1
    *  LPWSTR lpApplicationName
    ** LPWSTR lpCommandLine command_line
    *  LPSECURITY_ATTRIBUTES lpProcessAttributes
    *  LPSECURITY_ATTRIBUTES lpThreadAttributes
    ** BOOL bInheritHandles inherit_handles
    *  DWORD dwCreationFlags
    *  LPVOID lpEnvironment
    ** LPWSTR lpCurrentDirectory current_directory
    *  LPSTARTUPINFO lpStartupInfo
    *  LPPROCESS_INFORMATION lpProcessInformation
    *  LPVOID lpUnknown2

Flags::

    creation_flags creation_flags

Ensure::

    lpProcessInformation

Pre::

    // Ensure the CREATE_SUSPENDED flag is set when calling
    // the original function.
    DWORD creation_flags = dwCreationFlags;
    dwCreationFlags |= CREATE_SUSPENDED;

    wchar_t *filepath = get_unicode_buffer();
    path_get_full_pathW(lpApplicationName, filepath);

Interesting::

    u filepath
    u command_line
    i inherit_handles
    i creation_flags
    u current_directory

Middle::

    int track = 0;

    if(ret != FALSE) {
        uint32_t mode = HOOK_MODE_ALL;

        const wchar_t *command_line = lpCommandLine;
        if(command_line == NULL) {
            command_line = lpApplicationName;
        }

        // Let's ask nicely whether we want to propagate execution into this
        // new process and if so, in what monitoring mode.
        if(monitor_mode_should_propagate(command_line, &mode) == 0) {
            pipe("PROCESS2:%d,%d,%d",
                lpProcessInformation->dwProcessId,
                lpProcessInformation->dwThreadId,
                mode);
            track = 1;
        }
    }

Logging::

    u filepath filepath
    u filepath_r lpApplicationName
    i creation_flags creation_flags
    i process_identifier lpProcessInformation->dwProcessId
    i thread_identifier lpProcessInformation->dwThreadId
    p process_handle lpProcessInformation->hProcess
    p thread_handle lpProcessInformation->hThread
    i track track
    i stack_pivoted exploit_is_stack_pivoted()

Post::

    if(ret != FALSE) {
        // If the CREATE_SUSPENDED flag was not set then we have to resume
        // the main thread ourselves.
        if((creation_flags & CREATE_SUSPENDED) == 0) {
            ResumeThread(lpProcessInformation->hThread);
        }

        sleep_skip_disable();
    }

    free_unicode_buffer(filepath);

