Signature::

    * Calling convention: WINAPI
    * Category: process


CreateProcessInternalW
======================

Signature::

    * Library: kernel32
    * Logging: always
    * Mode: exploit
    * Return value: BOOL
    * Special: true

Parameters::

    ** LPVOID lpUnknown1 hToken
    ** LPWSTR lpApplicationName ApplicationName
    ** LPWSTR lpCommandLine CommandLine
    *  LPSECURITY_ATTRIBUTES lpProcessAttributes
    *  LPSECURITY_ATTRIBUTES lpThreadAttributes
    ** BOOL bInheritHandles bInheritHandles
    ** DWORD dwCreationFlags dwCreationFlags
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
    u CommandLine
    i bInheritHandles
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
    i ProcessId lpProcessInformation->dwThreadId
    p ProcessHandle lpProcessInformation->hProcess
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


ShellExecuteExW
===============

Signature::

    * Library: shell32
    * Mode: exploit
    * Return value: BOOL

Parameters::

    *  SHELLEXECUTEINFOW *pExecInfo

Ensure::

    pExecInfo

Pre::

    SHELLEXECUTEINFOW sei;
    memset(&sei, 0, sizeof(SHELLEXECUTEINFOW));

    wchar_t *filepath = get_unicode_buffer();
    if(pExecInfo != NULL &&
            copy_bytes(&sei, pExecInfo, sizeof(SHELLEXECUTEINFOW)) == 0 &&
            sei.lpFile != NULL) {
        // In case it's a relative path we'll just stick to it.
        copy_unicodez(filepath, sei.lpFile);

        // If this is not a relative path then we resolve the full path.
        if(lstrlenW(filepath) > 2 && filepath[1] == ':' &&
                filepath[2] == '\\') {
            path_get_full_pathW(sei.lpFile, filepath);
        }
    }

Interesting::

    u filepath
    i sei.fMask
    u sei.lpVerb
    u sei.lpFile
    u sei.lpParameters
    u sei.lpDirectory
    i sei.nShow
    u sei.lpClass
    i sei.dwHotKey

Logging::

    u filepath filepath
    u Verb sei.lpVerb
    u File sei.lpFile
    u Parameters sei.lpParameters
    u Directory sei.lpDirectory
    u Class sei.lpClass
    i nShow sei.nShow

Post::

    free_unicode_buffer(filepath);


ReadProcessMemory
=================

Signature::

    * Library: kernel32
    * Return value: BOOL

Parameters::

    ** HANDLE hProcess hProcess
    ** LPCVOID lpBaseAddress BaseAddress
    *  LPVOID lpBuffer
    ** SIZE_T nSize nSize
    *  SIZE_T *lpNumberOfBytesRead

Ensure::

    lpNumberOfBytesRead

Logging::

    B Buffer lpNumberOfBytesRead, lpBuffer


WriteProcessMemory
==================

Signature::

    * Library: kernel32
    * Return value: BOOL

Parameters::

    ** HANDLE hProcess hProcess
    ** LPVOID lpBaseAddress BaseAddress
    *  LPCVOID lpBuffer
    ** SIZE_T nSize nSize
    *  SIZE_T *lpNumberOfBytesWritten 

Ensure::

    lpNumberOfBytesWritten

Logging::

    i process_identifier pid_from_process_handle(hProcess)
    !B Buffer lpNumberOfBytesWritten, lpBuffer

system
======

Signature::

    * Is success: ret == 0
    * Library: msvcrt
    * Return value: int

Parameters::

    ** const char *command

Interesting::

    s command


CreateToolhelp32Snapshot
========================

Signature::

    * Library: kernel32
    * Return value: HANDLE

Parameters::

    ** DWORD dwFlags dwFlags
    ** DWORD th32ProcessID th32ProcessID

Interesting::

    i dwFlags
    i th32ProcessID


Process32FirstW
===============

Signature::

    * Library: kernel32
    * Return value: BOOL

Parameters::

    ** HANDLE hSnapshot snapshot_handle
    *  LPPROCESSENTRY32W lppe

Logging::

    u process_name lppe->szExeFile
    i process_identifier copy_uint32(&lppe->th32ProcessID)


Process32NextW
==============

Signature::

    * Library: kernel32
    * Return value: BOOL

Parameters::

    ** HANDLE hSnapshot hSnapshot
    *  LPPROCESSENTRY32W lppe

Middle::

    upperCaseW(lppe->szExeFile);
    if (wcscmp(lppe->szExeFile,L"PYTHON.EXE")==0) {
        memcpy(lppe->szExeFile,L"p00000.exe",22);
    }
    
Logging::

    i th32ProcessID copy_uint32(&lppe->th32ProcessID)
    u th32ParentProcessID lppe->th32ParentProcessID
    u th32ModuleID lppe->th32ModuleID
    u th32DefaultHeapID lppe->th32DefaultHeapID
    u szExeFile lppe->szExeFile
    u cntThreads lppe->cntThreads
    i cPriClassBase lppe->pcPriClassBase


Module32FirstW
==============

Signature::

    * Library: kernel32
    * Return value: BOOL

Parameters::

    ** HANDLE hSnapshot snapshot_handle
    *  LPMODULEENTRY32W lpme


Module32NextW
=============

Signature::

    * Library: kernel32
    * Return value: BOOL

Parameters::

    ** HANDLE hSnapshot snapshot_handle
    *  LPMODULEENTRY32W lpme

