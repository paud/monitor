Signature::

    * Calling convention: WINAPI
    * Category: services
    * Library: advapi32


OpenSCManagerA
==============

Signature::

    * Return value: SC_HANDLE

Parameters::

    ** LPCTSTR lpMachineName MachineName
    ** LPCTSTR lpDatabaseName DatabaseName
    ** DWORD dwDesiredAccess dwDesiredAccess

Interesting::

    s MachineName
    s DatabaseName
    i dwDesiredAccess


OpenSCManagerW
==============

Signature::

    * Return value: SC_HANDLE

Parameters::

    ** LPWSTR lpMachineName MachineName
    ** LPWSTR lpDatabaseName DatabaseName
    ** DWORD dwDesiredAccess dwDesiredAccess

Interesting::

    u MachineName
    u DatabaseName
    i dwDesiredAccess


CreateServiceA
==============

Signature::

    * Return value: SC_HANDLE

Parameters::

    ** SC_HANDLE hSCManager hSCManager
    ** LPCTSTR lpServiceName ServiceName 
    ** LPCTSTR lpDisplayName DisplayName
    ** DWORD dwDesiredAccess dwDesiredAccess
    ** DWORD dwServiceType dwServiceType
    ** DWORD dwStartType dwStartType
    ** DWORD dwErrorControl dwErrorControl
    ** LPCTSTR lpBinaryPathName BinaryPathName
    ** LPCTSTR lpLoadOrderGroup LoadOrderGroup
    *  LPDWORD lpdwTagId 
    ** LPCTSTR lpDependencies Dependencies
    ** LPCTSTR lpServiceStartName ServiceStartName
    ** LPCTSTR lpPassword Password

Pre::

    wchar_t *filepath = get_unicode_buffer();
    path_get_full_pathA(lpBinaryPathName, filepath);

Interesting::

    s ServiceName
    s DisplayName
    i dwDesiredAccess
    i dwServiceType
    i dwStartType
    i dwErrorControl
    u filepath
    s ServiceStartName
    s Password

Logging::

    p service_handle ret
    u filepath filepath
    s filepath_r lpBinaryPathName

Post::

    free_unicode_buffer(filepath);


CreateServiceW
==============

Signature::

    * Return value: SC_HANDLE

Parameters::

    ** SC_HANDLE hSCManager hSCManager
    ** LPWSTR lpServiceName ServiceName
    ** LPWSTR lpDisplayName DisplayName
    ** DWORD dwDesiredAccess dwDesiredAccess
    ** DWORD dwServiceType dwServiceType
    ** DWORD dwStartType dwStartType
    ** DWORD dwErrorControl dwErrorControl
    ** LPWSTR lpBinaryPathName BinaryPathName
    ** LPWSTR lpLoadOrderGroup LoadOrderGroup
    *  LPDWORD lpdwTagId
    ** LPWSTR lpDependencies Dependencies
    ** LPWSTR lpServiceStartName ServiceStartName
    ** LPWSTR lpPassword Password

Pre::

    wchar_t *filepath = get_unicode_buffer();
    path_get_full_pathW(lpBinaryPathName, filepath);

Interesting::

    u ServiceName
    u DisplayName
    i dwDesiredAccess
    i dwServiceType
    i dwStartType
    i dwErrorControl
    u BinaryPathName
    u ServiceStartName
    u Password

Logging::

    p service_handle ret
    u filepath filepath
    u filepath_r lpBinaryPathName

Post::

    free_unicode_buffer(filepath);


OpenServiceA
============

Signature::

    * Return value: SC_HANDLE

Parameters::

    ** SC_HANDLE hSCManager hSCManager
    ** LPCTSTR lpServiceName ServiceName
    ** DWORD dwDesiredAccess dwDesiredAccess

Interesting::

    s ServiceName
    i dwDesiredAccess

Logging::

    p service_handle ret


OpenServiceW
============

Signature::

    * Return value: SC_HANDLE

Parameters::

    ** SC_HANDLE hSCManager hSCManager
    ** LPWSTR lpServiceName ServiceName
    ** DWORD dwDesiredAccess dwDesiredAccess

Interesting::

    u ServiceName
    i dwDesiredAccess

Logging::

    p service_handle ret


StartServiceA
=============

Signature::

    * Return value: BOOL

Parameters::

    ** SC_HANDLE hService hService
    ** DWORD dwNumServiceArgs dwNumServiceArgs
    * LPCTSTR *lpServiceArgVectors ServiceArgVectors

Logging::

    a arguments dwNumServiceArgs, lpServiceArgVectors


StartServiceW
=============

Signature::

    * Return value: BOOL

Parameters::

    ** SC_HANDLE hService hService
    ** DWORD dwNumServiceArgs dwNumServiceArgs
    *  LPWSTR *lpServiceArgVectors ServiceArgVectors

Logging::

    A arguments dwNumServiceArgs, lpServiceArgVectors


ControlService
==============

Signature::

    * Return value: BOOL

Parameters::

    ** SC_HANDLE hService hService
    ** DWORD dwControl dwControl
    *  LPSERVICE_STATUS lpServiceStatus

Pre::

    #define DWORD_NULL						(DWORD)0
    DWORD dwServiceType;
    DWORD dwCurrentState;
    DWORD dwControlsAccepted;
    DWORD dwWin32ExitCode;
    DWORD dwServiceSpecificExitCode;
    DWORD dwCheckPoint,dwWaitHint;

    if (lpServiceStatus != NULL)
    {
        dwServiceType = lpServiceStatus->dwServiceType;
        dwCurrentState = lpServiceStatus->dwCurrentState;
        dwControlsAccepted = lpServiceStatus->dwCurrentState;
        dwWin32ExitCode = lpServiceStatus->dwWin32ExitCode;
        dwServiceSpecificExitCode = lpServiceStatus->dwServiceSpecificExitCode;
        dwCheckPoint = lpServiceStatus->dwCheckPoint;
        dwWaitHint = lpServiceStatus->dwWaitHint;
    } 
    else
    {
        dwServiceType = DWORD_NULL;
        dwCurrentState = DWORD_NULL;
        dwControlsAccepted = DWORD_NULL;
        dwWin32ExitCode = DWORD_NULL;
        dwServiceSpecificExitCode = DWORD_NULL;
        dwCheckPoint = DWORD_NULL;
        dwWaitHint = DWORD_NULL;
    }
    

Logging::

    i dwServiceType dwServiceType
    i dwCurrentState dwCurrentState
    i dwControlsAccepted dwControlsAccepted
    i dwWin32ExitCode dwWin32ExitCode
    i dwServiceSpecificExitCode dwServiceSpecificExitCode
    i dwCheckPoint dwCheckPoint
    i dwWaitHint dwWaitHint


DeleteService
=============

Signature::

    * Return value: BOOL

Parameters::

    ** SC_HANDLE hService hService


EnumServicesStatusA
===================

Signature::

    * Return value: BOOL

Parameters::

    ** SC_HANDLE hSCManager service_handle
    ** DWORD dwServiceType service_type
    ** DWORD dwServiceState service_status
    *  LPENUM_SERVICE_STATUS lpServices
    *  DWORD cbBufSize
    *  LPDWORD pcbBytesNeeded
    *  LPDWORD lpServicesReturned
    *  LPDWORD lpResumeHandle

Middle::

    //string str=malloc(1024*1024);
    //memset(str,0,1024*1024);
    //char str[1024]={0};
	//for (int i = 0; i < lpServicesReturned; i++)
	//{cant make output infomation, it will be broken
		//stradd(str,lpServices[i].lpDisplayName);
        //stradd(str,",");
	//}
    //str[length(str) - 1] = 0;
    //char str[10]="1\n";

Logging::

    i DisplayNameList lpServicesReturned


EnumServicesStatusW
===================

Signature::

    * Return value: BOOL

Parameters::

    ** SC_HANDLE hSCManager service_handle
    ** DWORD dwServiceType service_type
    ** DWORD dwServiceState service_status
    *  LPENUM_SERVICE_STATUS lpServices
    *  DWORD cbBufSize
    *  LPDWORD pcbBytesNeeded
    *  LPDWORD lpServicesReturned
    *  LPDWORD lpResumeHandle


StartServiceCtrlDispatcherW
===========================

Signature::

    * Return value: BOOL

Parameters::

    *  const SERVICE_TABLE_ENTRYW *lpServiceTable

Pre::

    bson b, a; char index[10]; int idx = 0; SERVICE_TABLE_ENTRYW entry;
    bson_init(&b);
    bson_init(&a);

    bson_append_start_array(&b, "services");
    bson_append_start_array(&a, "addresses");

    const SERVICE_TABLE_ENTRYW *ptr = lpServiceTable;
    while (
        copy_bytes(&entry, ptr, sizeof(SERVICE_TABLE_ENTRYW)) == 0 &&
        entry.lpServiceProc != NULL
    ) {
        our_snprintf(index, sizeof(index), "%d", idx++);
        log_wstring(&b, index, entry.lpServiceName,
            copy_strlenW(entry.lpServiceName));

        log_intptr(&a, index, (intptr_t)(uintptr_t) entry.lpServiceProc);
    }

    bson_append_finish_array(&a);
    bson_append_finish_array(&b);
    bson_finish(&a);
    bson_finish(&b);

Logging::

    z addresses &a
    z services &b

Post::

    bson_destroy(&a);
    bson_destroy(&b);
