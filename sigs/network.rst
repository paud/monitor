Signature::

    * Calling convention: WINAPI
    * Category: network


URLDownloadToFileW
==================

Signature::

    * Library: urlmon
    * Mode: exploit
    * Return value: HRESULT

Parameters::

    *  LPUNKNOWN pCaller
    ** LPWSTR szURL szURL
    *  LPWSTR szFileName
    *  DWORD dwReserved
    *  LPVOID lpfnCB

Interesting::

    u szURL
    u filepath

Pre::

    wchar_t *filepath = get_unicode_buffer();
    path_get_full_pathW(szFileName, filepath);

Logging::

    u filepath filepath
    u szFileName szFileName
    i stack_pivoted exploit_is_stack_pivoted()

Post::

    pipe("FILE_NEW:%Z", filepath);

    free_unicode_buffer(filepath);


InternetCrackUrlA
=================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    *  LPCSTR lpszUrl
    *  DWORD dwUrlLength
    ** DWORD dwFlags flags
    *  LPURL_COMPONENTSA lpUrlComponents

Pre::

    uint32_t length = dwUrlLength;
    if(length == 0 && lpszUrl != NULL) {
        length = copy_strlen(lpszUrl);
    }

Logging::

    S url length, lpszUrl


InternetCrackUrlW
=================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    *  LPCWSTR lpszUrl
    *  DWORD dwUrlLength
    ** DWORD dwFlags flags
    *  LPURL_COMPONENTSW lpUrlComponents

Pre::

    uint32_t length = dwUrlLength;
    if(length == 0 && lpszUrl != NULL) {
        length = copy_strlenW(lpszUrl);
    }

Logging::

    U url length, lpszUrl


InternetOpenA
=============

Signature::

    * Library: wininet
    * Return value: HINTERNET

Parameters::

    ** LPCTSTR lpszAgent lpszAgent
    ** DWORD dwAccessType dwAccessType
    ** LPCTSTR lpszProxyName lpszProxyName
    ** LPCTSTR lpszProxyBypass lpszProxyBypass
    ** DWORD dwFlags dwFlags

Interesting::

    s lpszAgent
    i dwAccessType
    s lpszProxyName
    s lpszProxyBypass
    i dwFlags


InternetOpenW
=============

Signature::

    * Library: wininet
    * Return value: HINTERNET

Parameters::

    ** LPWSTR lpszAgent user_agent
    ** DWORD dwAccessType access_type
    ** LPWSTR lpszProxyName proxy_name
    ** LPWSTR lpszProxyBypass proxy_bypass
    ** DWORD dwFlags flags

Interesting::

    u user_agent
    i access_type
    u proxy_name
    u proxy_bypass
    i flags


InternetConnectA
================

Signature::

    * Library: wininet
    * Return value: HINTERNET

Parameters::

    ** HINTERNET hInternet internet_handle
    ** LPCTSTR lpszServerName szServerName
    ** INTERNET_PORT nServerPort nServerPort
    ** LPCTSTR lpszUsername szUsername
    ** LPCTSTR lpszPassword szPassword
    ** DWORD dwService dwService
    ** DWORD dwFlags dwFlags
    *  DWORD_PTR dwContext

Interesting::

    s szServerName
    i nServerPort
    s szUsername
    s szPassword
    i dwService
    i dwFlags

Logging::

    s dwContext dwContext


InternetConnectW
================

Signature::

    * Library: wininet
    * Return value: HINTERNET

Parameters::

    ** HINTERNET hInternet internet_handle
    ** LPWSTR lpszServerName hostname
    ** INTERNET_PORT nServerPort port
    ** LPWSTR lpszUsername username
    ** LPWSTR lpszPassword password
    ** DWORD dwService service
    ** DWORD dwFlags flags
    *  DWORD_PTR dwContext

Interesting::

    u hostname
    i port
    u username
    u password
    i service
    i flags


InternetOpenUrlA
================

Signature::

    * Library: wininet
    * Return value: HINTERNET

Parameters::

    ** HINTERNET hInternet hInternet
    ** LPCTSTR lpszUrl szUrl 
    ** LPCTSTR lpszHeaders szHeaders
    ** DWORD dwHeadersLength dwHeadersLength
    ** DWORD dwFlags dwFlags
    *  DWORD_PTR dwContext

Pre::

    int headers_length = dwHeadersLength;
    if(lpszHeaders != NULL && headers_length == -1) {
        headers_length = copy_strlen(lpszHeaders);
    }

Interesting::

    s szUrl
    S headers_length, lpszHeaders
    i dwFlags

Logging::

    S headers headers_length, lpszHeaders
    u dwContext dwContext


InternetOpenUrlW
================

Signature::

    * Library: wininet
    * Return value: HINTERNET

Parameters::

    ** HINTERNET hInternet
    ** LPWSTR lpszUrl url
    *  LPWSTR lpszHeaders
    *  DWORD dwHeadersLength
    ** DWORD dwFlags flags
    *  DWORD_PTR dwContext

Pre::

    int headers_length = dwHeadersLength;
    if(lpszHeaders != NULL && headers_length == -1) {
        headers_length = copy_strlenW(lpszHeaders);
    }

Interesting::

    u url
    U headers_length, lpszHeaders
    i flags

Logging::

    U headers headers_length, lpszHeaders


InternetQueryOptionA
====================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** HINTERNET hInternet internet_handle
    ** DWORD dwOption option
    *  LPVOID lpBuffer
    *  LPDWORD lpdwBufferLength

Flags::

    option


InternetSetOptionA
==================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** HINTERNET hInternet internet_handle
    ** DWORD dwOption option
    *  LPVOID lpBuffer
    *  DWORD dwBufferLength

Flags::

    option


HttpOpenRequestA
================

Signature::

    * Library: wininet
    * Return value: HINTERNET

Parameters::

    ** HINTERNET hConnect hHttpSession
    ** LPCTSTR lpszVerb szVerb
    ** LPCTSTR lpszObjectName szObjectName
    ** LPCTSTR lpszVersion szVersion
    ** LPCTSTR lpszReferer szReferer
    ** LPCTSTR *lplpszAcceptTypes szAcceptTypes
    ** DWORD dwFlags dwFlags
    *  DWORD_PTR dwContext 

Interesting::

    s szVerb
    s szObjectName
    s szVersion
    s szReferer
    i dwFlags


HttpOpenRequestW
================

Signature::

    * Library: wininet
    * Return value: HINTERNET

Parameters::

    ** HINTERNET hConnect hHttpSession
    ** LPWSTR lpszVerb szVerb
    ** LPWSTR lpszObjectName szObjectName
    ** LPWSTR lpszVersion szVersion
    ** LPWSTR lpszReferer szReferer
    ** LPWSTR *lplpszAcceptTypes szAcceptTypes
    ** DWORD dwFlags flags
    *  DWORD_PTR dwContext

Interesting::

    s szVerb
    s szObjectName
    s szVersion
    s szReferer
    i dwFlags


HttpSendRequestA
================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** HINTERNET hRequest request_handle
    *  LPCTSTR lpszHeaders
    *  DWORD dwHeadersLength
    *  LPVOID lpOptional
    *  DWORD dwOptionalLength

Pre::

    int headers_length = dwHeadersLength;
    if(lpszHeaders != NULL && headers_length == -1) {
        headers_length = copy_strlen(lpszHeaders);
    }

Interesting::

    S dwHeadersLength, lpszHeaders
    b dwOptionalLength, lpOptional

Logging::

    S headers headers_length, lpszHeaders
    b post_data (uintptr_t) dwOptionalLength, lpOptional


HttpSendRequestW
================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** HINTERNET hRequest request_handle
    *  LPWSTR lpszHeaders
    *  DWORD dwHeadersLength
    *  LPVOID lpOptional
    *  DWORD dwOptionalLength

Pre::

    int headers_length = dwHeadersLength;
    if(lpszHeaders != NULL && headers_length == -1) {
        headers_length = copy_strlenW(lpszHeaders);
    }

Interesting::

    U dwHeadersLength, lpszHeaders
    b dwOptionalLength, lpOptional

Logging::

    U headers headers_length, lpszHeaders
    b post_data (uintptr_t) dwOptionalLength, lpOptional


InternetReadFile
================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** HINTERNET hFile hFile
    *  LPVOID lpBuffer
    ** DWORD dwNumberOfBytesToRead dwNumberOfBytesToRead
    *  LPDWORD lpdwNumberOfBytesRead

Ensure::

    lpdwNumberOfBytesRead

Logging::

    b Buffer (uintptr_t) copy_uint32(lpdwNumberOfBytesRead), lpBuffer


InternetWriteFile
=================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** HINTERNET hFile request_handle
    *  LPCVOID lpBuffer
    *  DWORD dwNumberOfBytesToWrite
    *  LPDWORD lpdwNumberOfBytesWritten

Ensure::

    lpdwNumberOfBytesWritten

Logging::

    b buffer (uintptr_t) copy_uint32(lpdwNumberOfBytesWritten), lpBuffer


InternetCloseHandle
===================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** HINTERNET hInternet internet_handle


InternetGetConnectedState
=========================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** LPDWORD lpdwFlags flags
    *  DWORD dwReserved


InternetGetConnectedStateExA
============================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** LPDWORD lpdwFlags flags
    ** LPCSTR lpszConnectionName connection_name
    *  DWORD dwNameLen
    *  DWORD dwReserved


InternetGetConnectedStateExW
============================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** LPDWORD lpdwFlags flags
    ** LPWSTR lpszConnectionName connection_name
    *  DWORD dwNameLen
    *  DWORD dwReserved


InternetSetStatusCallback
=========================

Signature::

    * Is success: 1
    * Library: wininet
    * Return value: INTERNET_STATUS_CALLBACK

Parameters::

    ** HINTERNET hInternet internet_handle
    ** INTERNET_STATUS_CALLBACK lpfnInternetCallback callback


DeleteUrlCacheEntryA
====================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** LPCSTR lpszUrlName url


DeleteUrlCacheEntryW
====================

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** LPWSTR lpszUrlName url


DnsQuery_A
==========

Signature::

    * Library: dnsapi
    * Return value: DNS_STATUS

Parameters::

    ** PCSTR lpstrName hostname
    ** WORD wType dns_type
    ** DWORD Options options
    *  PVOID pExtra
    *  PDNS_RECORD *ppQueryResultsSet
    *  PVOID *pReserved

Interesting::

    s hostname
    i dns_type
    i options


DnsQuery_UTF8
=============

Signature::

    * Library: dnsapi
    * Return value: DNS_STATUS

Parameters::

    *  LPBYTE lpstrName
    ** WORD wType dns_type
    ** DWORD Options options
    *  PVOID pExtra
    *  PDNS_RECORD *ppQueryResultsSet
    *  PVOID *pReserved

Interesting::

    s lpstrName
    i dns_type
    i options

Logging::

    s hostname lpstrName


DnsQuery_W
==========

Signature::

    * Library: dnsapi
    * Return value: DNS_STATUS

Parameters::

    ** PWSTR lpstrName hostname
    ** WORD wType dns_type
    ** DWORD Options options
    *  PVOID pExtra
    *  PDNS_RECORD *ppQueryResultsSet
    *  PVOID *pReserved

Interesting::

    u hostname
    i dns_type
    i options


getaddrinfo
===========

Signature::

    * Is success: ret == 0
    * Library: ws2_32
    * Return value: int

Parameters::

    ** PCSTR pNodeName hostname
    ** PCSTR pServiceName service_name
    *  const ADDRINFOA *pHints
    *  PADDRINFOA *ppResult

Interesting::

    s hostname
    s service_name


GetAddrInfoW
============

Signature::

    * Is success: ret == 0
    * Library: ws2_32
    * Return value: int

Parameters::

    ** PCWSTR pNodeName hostname
    ** PCWSTR pServiceName service_name
    *  const ADDRINFOW *pHints
    *  PADDRINFOW *ppResult

Interesting::

    u hostname
    u service_name


GetInterfaceInfo
================

Signature::

    * Is success: ret == NO_ERROR
    * Library: iphlpapi
    * Return value: DWORD

Parameters::

    *  PIP_INTERFACE_INFO pIfTable
    *  PULONG dwOutBufLen


GetAdaptersInfo
===============

Signature::

    * Is success: ret == NO_ERROR
    * Library: iphlpapi
    * Return value: DWORD

Parameters::

    *  PIP_ADAPTER_INFO pAdapterInfo
    *  PULONG pOutBufLen

Logging::
    
    s AdapterNmae pAdapterInfo->AdapterName
    u AdapterAdress pAdapterInfo->Address
    u ADescription pAdapterInfo->Type


GetAdaptersAddresses
====================

Signature::

    * Is success: ret == ERROR_SUCCESS
    * Library: iphlpapi
    * Return value: ULONG

Parameters::

    ** ULONG Family family
    ** ULONG Flags flags
    *  PVOID Reserved
    *  PIP_ADAPTER_ADDRESSES AdapterAddresses
    *  PULONG SizePointer


HttpQueryInfoA
==============

Signature::

    * Library: wininet
    * Return value: BOOL

Parameters::

    ** HINTERNET hRequest request_handle
    ** DWORD dwInfoLevel info_level
    *  LPVOID lpvBuffer
    *  LPDWORD lpdwBufferLength
    ** LPDWORD lpdwIndex index

Ensure::

    lpdwBufferLength

Logging::

    b buffer (uintptr_t) copy_uint32(lpdwBufferLength), lpvBuffer


ObtainUserAgentString
=====================

Signature::

    * Library: urlmon
    * Return value: HRESULT

Parameters::

    ** DWORD dwOption option
    *  LPSTR pcszUAOut
    *  DWORD *cbSize

Ensure::

    cbSize

Middle::

    uint32_t length = ret == S_OK ? copy_uint32(cbSize) : 0;

Logging::

    S user_agent length, pcszUAOut


GetBestInterfaceEx
==================

Signature::

    * Is success: ret == NO_ERROR
    * Library: iphlpapi
    * Return value: DWORD

Parameters::

    *  struct sockaddr *pDestAddr
    *  PDWORD pdwBestIfIndex


WNetGetProviderNameW
====================

Signature::

    * Is success: ret == NO_ERROR
    * Library: mpr
    * Return value: DWORD

Parameters::

    *  DWORD dwNetType
    *  LPTSTR lpProviderName
    *  LPDWORD lpBufferSize

Ensure::

    lpBufferSize

Logging::

    x net_type dwNetType
