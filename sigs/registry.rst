Signature::

    * Calling convention: WINAPI
    * Category: registry
    * Is success: ret == ERROR_SUCCESS
    * Library: advapi32
    * Return value: LONG


RegOpenKeyExA
=============

Parameters::

    ** HKEY hKey hKey
    ** LPCTSTR lpSubKey SubKey
    ** DWORD ulOptions options
    ** REGSAM samDesired samDesired
    ** PHKEY phkResult hkResult

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_asciiz(hKey, lpSubKey, regkey);

Interesting::

    u regkey
    i options
    i samDesired

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegOpenKeyExW
=============

Parameters::

    ** HKEY hKey hKey
    ** LPWSTR lpSubKey SubKey
    ** DWORD ulOptions options
    ** REGSAM samDesired samDesired
    ** PHKEY phkResult hkResult

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_uniz(hKey, lpSubKey, regkey);

Interesting::

    u regkey
    i options
    i samDesired

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegCreateKeyExA
===============

Parameters::

    ** HKEY hKey hKey
    ** LPCTSTR lpSubKey SubKey
    *  DWORD Reserved
    ** LPTSTR lpClass Class
    ** DWORD dwOptions options
    ** REGSAM samDesired samDesired
    *  LPSECURITY_ATTRIBUTES lpSecurityAttributes
    ** PHKEY phkResult hkResult
    ** LPDWORD lpdwDisposition dwDisposition

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_asciiz(hKey, lpSubKey, regkey);

Interesting::

    u regkey
    s Class
    i options
    i samDesired
    I dwDisposition

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegCreateKeyExW
===============

Parameters::

    ** HKEY hKey hKey
    ** LPWSTR lpSubKey SubKey
    *  DWORD Reserved
    ** LPWSTR lpClass Class
    ** DWORD dwOptions options
    ** REGSAM samDesired samDesired
    *  LPSECURITY_ATTRIBUTES lpSecurityAttributes
    ** PHKEY phkResult hkResult
    ** LPDWORD lpdwDisposition dwDisposition

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_uniz(hKey, lpSubKey, regkey);

Interesting::

    u regkey
    u Class
    i options
    i samDesired
    I dwDisposition

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegDeleteKeyA
=============

Parameters::

    ** HKEY hKey hKey
    ** LPCTSTR lpSubKey SubKey

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_asciiz(hKey, lpSubKey, regkey);

Interesting::

    u regkey

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegDeleteKeyW
=============

Parameters::

    ** HKEY hKey hKey
    ** LPWSTR lpSubKey SubKey

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_uniz(hKey, lpSubKey, regkey);

Interesting::

    u regkey

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegEnumKeyW
===========

Parameters::

    ** HKEY hKey key_handle
    ** DWORD dwIndex index
    ** LPWSTR lpName key_name
    *  DWORD cchName

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key(hKey, regkey);

Interesting::

    u regkey
    i index

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegEnumKeyExA
=============

Parameters::

    ** HKEY hKey hKey
    ** DWORD dwIndex dwIndex
    ** LPTSTR lpName Name
    ** LPDWORD lpcName cName
    *  LPDWORD lpReserved
    ** LPTSTR lpClass Class
    ** LPDWORD lpcClass cClass
    *  PFILETIME lpftLastWriteTime LastWriteTime

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key(hKey, regkey);

Interesting::

    u regkey
    i dwIndex

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegEnumKeyExW
=============

Parameters::

    ** HKEY hKey hKey
    ** DWORD dwIndex dwIndex
    ** LPWSTR lpName Name
    ** LPDWORD lpcName cName
    *  LPDWORD lpReserved
    ** LPWSTR lpClass Class
    ** LPDWORD lpcClass cClass
    *  PFILETIME lpftLastWriteTime LastWriteTime

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key(hKey, regkey);

Interesting::

    u regkey
    i dwIndex

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegEnumValueA
=============

Parameters::

    ** HKEY hKey hKey
    ** DWORD dwIndex dwIndex
    ** LPTSTR lpValueName ValueName
    ** LPDWORD lpcValueName cValueName
    *  LPDWORD lpReserved 
    ** LPDWORD lpType dwType
    *  LPBYTE lpData
    ** LPDWORD lpcbData cbData

Flags::

    dwType

Ensure::

    lpType
    lpcbData

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_asciiz(hKey, lpValueName, regkey);

    *lpType = REG_NONE;

Logging::

    u regkey regkey
    r Data lpType, lpcbData, lpData

Post::
    
    free_unicode_buffer(regkey);


RegEnumValueW
=============

Parameters::

    ** HKEY hKey hKey
    ** DWORD dwIndex dwIndex
    ** LPWSTR lpValueName ValueName
    ** LPDWORD lpcValueName cValueName
    *  LPDWORD lpReserved
    ** LPDWORD lpType dwType
    *  LPBYTE lpData
    ** LPDWORD lpcbData cbData

Flags::

    dwType

Ensure::

    lpType
    lpcbData

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_uniz(hKey, lpValueName, regkey);

    *lpType = REG_NONE;

Logging::

    u regkey regkey
    R Data lpType, lpcbData, lpData

Post::

    free_unicode_buffer(regkey);


RegSetValueExA
==============

Parameters::

    ** HKEY hKey hKey
    ** LPCTSTR lpValueName ValueName
    *  DWORD Reserved
    ** DWORD dwType dwType
    *  const BYTE *lpData
    ** DWORD cbData cbDate

Flags::

    dwType

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_asciiz(hKey, lpValueName, regkey);

Interesting::

    u regkey
    i dwType
    b cbData, lpData

Logging::

    u regkey regkey
    r Data &dwType, &cbData, lpData

Post::

    free_unicode_buffer(regkey);


RegSetValueExW
==============

Parameters::

    ** HKEY hKey hKey
    ** LPWSTR lpValueName ValueName
    *  DWORD Reserved
    ** DWORD dwType dwType
    *  const BYTE *lpData
    ** DWORD cbData cbData

Flags::

    dwType

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_uniz(hKey, lpValueName, regkey);

Interesting::

    u regkey
    i dwType
    b cbData, lpData

Logging::

    u regkey regkey
    R Data &dwType, &cbData, lpData

Post::

    free_unicode_buffer(regkey);


RegQueryValueExA
================

Parameters::

    ** HKEY hKey hKey
    ** LPCTSTR lpValueName ValueName
    *  LPDWORD lpReserved
    ** LPDWORD lpType dwType
    *  LPBYTE lpData
    ** LPDWORD lpcbData cbData

Flags::

    dwType

Ensure::

    lpType
    lpcbData

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_asciiz(hKey, lpValueName, regkey);

    *lpType = REG_NONE;

Interesting::

    u regkey

Logging::

    u regkey regkey
    r Data lpType, lpcbData, lpData

Post::

    free_unicode_buffer(regkey);


__RegQueryValueExW
================

Parameters::

    ** HKEY hKey hKey
    ** LPWSTR lpValueName ValueName
    *  LPDWORD lpReserved
    ** LPDWORD lpType dwType
    *  LPBYTE lpData
    ** LPDWORD lpcbData cbData

Flags::

    dwType

Ensure::

    lpType
    lpcbData

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_uniz(hKey, lpValueName, regkey);

    *lpType = REG_NONE;

Interesting::

    u regkey

Logging::

    u regkey regkey
    R Data lpType, lpcbData, lpData

Post::

    free_unicode_buffer(regkey);


RegDeleteValueA
===============

Parameters::

    ** HKEY hKey key_handle
    ** LPCTSTR lpValueName regkey_r

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_asciiz(hKey, lpValueName, regkey);

Interesting::

    u regkey

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegDeleteValueW
===============

Parameters::

    ** HKEY hKey key_handle
    ** LPWSTR lpValueName regkey_r

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key_uniz(hKey, lpValueName, regkey);

Interesting::

    u regkey

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegQueryInfoKeyA
================

Parameters::

    ** HKEY hKey key_handle
    ** LPTSTR lpClass class
    *  LPDWORD lpcClass
    *  LPDWORD lpReserved
    ** LPDWORD lpcSubKeys subkey_count
    ** LPDWORD lpcMaxSubKeyLen subkey_max_length
    ** LPDWORD lpcMaxClassLen class_max_length
    ** LPDWORD lpcValues value_count
    ** LPDWORD lpcMaxValueNameLen value_name_max_length
    ** LPDWORD lpcMaxValueLen value_max_length
    *  LPDWORD lpcbSecurityDescriptor
    *  PFILETIME lpftLastWriteTime

Ensure::

    lpcSubKeys
    lpcMaxSubKeyLen
    lpcMaxClassLen
    lpcValues
    lpcMaxValueNameLen
    lpcMaxValueLen

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key(hKey, regkey);

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegQueryInfoKeyW
================

Parameters::

    ** HKEY hKey key_handle
    ** LPWSTR lpClass class
    *  LPDWORD lpcClass
    *  LPDWORD lpReserved
    ** LPDWORD lpcSubKeys subkey_count
    ** LPDWORD lpcMaxSubKeyLen subkey_max_length
    ** LPDWORD lpcMaxClassLen class_max_length
    ** LPDWORD lpcValues value_count
    ** LPDWORD lpcMaxValueNameLen value_name_max_length
    ** LPDWORD lpcMaxValueLen value_max_length
    *  LPDWORD lpcbSecurityDescriptor
    *  PFILETIME lpftLastWriteTime

Ensure::

    lpcSubKeys
    lpcMaxSubKeyLen
    lpcMaxClassLen
    lpcValues
    lpcMaxValueNameLen
    lpcMaxValueLen

Pre::

    wchar_t *regkey = get_unicode_buffer();
    reg_get_key(hKey, regkey);

Logging::

    u regkey regkey

Post::

    free_unicode_buffer(regkey);


RegCloseKey
===========

Parameters::

    ** HKEY hKey key_handle
