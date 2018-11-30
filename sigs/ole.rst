Signature::

    * Calling convention: WINAPI
    * Category: ole
    * Library: ole32
    * Mode: exploit
    * Return value: HRESULT


CoCreateInstance
================

Parameters::

    ** REFCLSID rclsid rclsid
    *  LPUNKNOWN pUnkOuter
    ** DWORD dwClsContext class_context
    ** REFIID riid riid
    *  LPVOID *ppv

Interesting::

    b sizeof(CLSID), rclsid
    i class_context
    b sizeof(IID), riid

pre::

    char *cCLSID = NULL;
    char *rRIID = NULL;

    cCLSID = (char *)malloc(128+3); 
    rRIID = (char *)malloc(128+3);
    wsprintf(cCLSID, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", rclsid->Data1, 
              rclsid->Data2, rclsid->Data3, rclsid->Data4[0], rclsid->Data4[1], rclsid->Data4[2], 
              rclsid->Data4[3], rclsid->Data4[4], rclsid->Data4[5], rclsid->Data4[6], rclsid->Data4[7]);
    
    wsprintf(rRIID,"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", riid->Data1,
             riid->Data2, riid->Data3, riid->Data4[0], riid->Data4[1], riid->Data4[2],
             riid->Data4[3], riid->Data4[4], riid->Data4[5], riid->Data4[6], riid->Data4[7]);

Post::

    ole_enable_hooks(rclsid);
    free(cCLSID);
    free(rRIID);

Logging::
 
    s cCLSID cCLSID
    s rRIID rRIID


OleInitialize
=============

Parameters::

    *  LPVOID pvReserved


CoInitializeEx
==============

Parameters::

    *  LPVOID pvReserved
    ** DWORD dwCoInit options


CoUninitialize
==============


CoInitializeSecurity
====================

Parameters::

    *  PSECURITY_DESCRIPTOR pSecDesc
    *  LONG cAuthSvc
    *  SOLE_AUTHENTICATION_SERVICE *asAuthSvc
    *  void *pReserved1
    *  DWORD dwAuthnLevel
    *  DWORD dwImpLevel
    *  void *pAuthList
    *  DWORD dwCapabilities
    *  void *pReserved3


CoCreateInstanceEx
==================

Parameters::

    *  REFCLSID rclsid
    *  IUnknown *punkOuter
    ** DWORD dwClsCtx dwClsCtx
    *  COSERVERINFO *pServerInfo
    *  DWORD dwCount
    *  MULTI_QI *pResults

Pre::

    bson b; char index[8], clsid[64];
    bson_init(&b);

    bson_append_start_array(&b, "iid");

    MULTI_QI *multi_qi = pResults;
    for (uint32_t idx = 0; idx < dwCount; idx++, multi_qi++) {
        our_snprintf(index, sizeof(index), "%d", idx++);
        clsid_to_string(copy_ptr(&multi_qi->pIID), clsid);
        log_string(&b, index, clsid, our_strlen(clsid));
    }

    bson_append_finish_array(&b);
    bson_finish(&b);
    
    LPOLESTR lpProgID = NULL;
    ProgIDFromCLSID(rclsid, &lpProgID);
    char *CLSID = NULL;

    CLSID = (char *)malloc(128+3); 
    wsprintf(CLSID, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", rclsid->Data1, 
              rclsid->Data2, rclsid->Data3, rclsid->Data4[0], rclsid->Data4[1], rclsid->Data4[2], 
              rclsid->Data4[3], rclsid->Data4[4], rclsid->Data4[5], rclsid->Data4[6], rclsid->Data4[7]);

Logging::

    z iid &b
    s ProgId lpProgID
    s CLSID CLSID

Post::

    ole_enable_hooks(rclsid);
    bson_destroy(&b);
    free(CLSID);


CoGetClassObject
================

Parameters::

    ** REFCLSID rclsid CLSID
    ** DWORD dwClsContext dwClsCtx
    *  COSERVERINFO *pServerInfo
    ** REFIID riid InterfaceId
    *  LPVOID *ppv

Post::

    ole_enable_hooks(rclsid);

Logging::
    
    s MachineName pServerInfo != NULL ? pServerInfo->pwszName : L"(null)"


OleConvertOLESTREAMToIStorage
=============================

Parameters::

    *  LPOLESTREAM lpolestream
    *  IStorage *pstg
    *  const DVTARGETDEVICE *ptd

Middle::

    void *buf = NULL; uintptr_t len = 0;

    #if !__x86_64__
    if(lpolestream != NULL) {
        buf = copy_ptr(copy_ptr((uint8_t *) lpolestream + 8));
        len = copy_uint32((uint8_t *) lpolestream + 12);
    }
    #endif

Logging::

    !b ole2 len, buf
