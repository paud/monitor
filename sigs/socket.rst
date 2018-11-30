Signature::

    * Calling convention: WINAPI
    * Category: network
    * Library: ws2_32


WSAStartup
==========

Signature::

    * Is success: ret == 0
    * Return value: int

Parameters::

    ** WORD wVersionRequested
    *  LPWSADATA lpWSAData

Interesting::

    i wVersionRequested

Logging::
    
    u wVersion (DWORD)lpWSAData->wVersion
    u wHighVersion (DWORD)lpWSAData->wHighVersion
    s szDescription lpWSAData->szDescription
    s szSystemStatus lpWSAData->szSystemStatus


_gethostbyname
=============

Signature::

    * Is success: ret != NULL
    * Return value: struct hostent *

Parameters::

    ** const char *name name

Interesting::

    s name

Middle::

    SOCKADDR_IN addr;
    addr.sin_addr.S_un.S_addr =  *(ULONG*)ret->h_addr_list[0];
    char * addr_str = inet_ntoa(addr.sin_addr);
    
Logging::
    
    s addr addr_str

socket
======

Signature::

    * Return value: SOCKET

Parameters::

    ** int af
    ** int type
    ** int protocol

Interesting::

    i af
    i type
    i protocol

Logging::

    i socket ret


getsockname
===========

Signature::

    * Is success: ret != SOCKET_ERROR
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  struct sockaddr *name
    *  int *namelen

Middle::

    const char *ip = NULL; int port = 0;
    get_ip_port(name, &ip, &port);

Logging::

    s ip_address ip
    i port port
    u namelen namelen


connect
=======

Signature::

    * Is success: ret != SOCKET_ERROR
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  const struct sockaddr *name
    *  int namelen

Pre::

    const char *ip = NULL; int port = 0;
    get_ip_port(name, &ip, &port);

Interesting::

    s ip
    i port

Post::

   if(netsimulation == 1) {
        ret = 0;
   }
   
Logging::

    s addr ip
    i port port


send
====

Signature::

    * Is success: ret > 0
    * Return value: int

Parameters::

    ** SOCKET s socket
    ** const char *buf buf_addr
    ** int len len
    *  int flags

Logging::

    i sent ret
    !b buf (uintptr_t)(ret > 0 ? ret : 0), buf


sendto
======

Signature::

    * Is success: ret > 0
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  const char *buf
    ** int len len
    ** int flags flags
    *  const struct sockaddr *to
    *  int tolen

Pre::

    const char *ip = NULL; int port = 0;
    get_ip_port(to, &ip, &port);

Logging::

    s addr ip
    i port port
    i sent ret
    !b buff (uintptr_t)(ret > 0 ? ret : 0), buf


recv
====

Signature::

    * Is success: ret > 0
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  char *buf 
    ** int len len
    ** int flags flags

Logging::

    i received ret
    !b buffer (uintptr_t)(ret > 0 ? ret : 0), buf


recvfrom
========

Signature::

    * Is success: ret > 0
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  char *buf
    ** int len len
    ** int flags flags
    *  struct sockaddr *from
    *  int *fromlen

Middle::

    const char *ip = NULL; int port = 0;
    get_ip_port(from, &ip, &port);

Logging::

    s addr ip
    i port port
    !b buf (uintptr_t)(ret > 0 ? ret : 0), buf


accept
======

Signature::

    * Return value: SOCKET

Parameters::

    ** SOCKET s socket
    *  struct sockaddr *addr
    *  int *addrlen

Middle::

    const char *ip = NULL; int port = 0;
    get_ip_port(addr, &ip, &port);

Logging::

    s ip_address ip
    i port port


bind
====

Signature::

    * Is success: ret != SOCKET_ERROR
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  const struct sockaddr *name
    *  int namelen

Pre::

    const char *ip = NULL; int port = 0;
    get_ip_port(name, &ip, &port);

Logging::

    s addr ip
    i port port


listen
======

Signature::

    * Is success: ret != SOCKET_ERROR
    * Return value: int

Parameters::

    ** SOCKET s socket
    ** int backlog backlog


select
======

Signature::

    * Is success: ret != SOCKET_ERROR
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  fd_set *readfds
    *  fd_set *writefds
    *  fd_set *exceptfds
    *  const struct timeval *timeout


setsockopt
==========

Signature::

    * Is success: ret != SOCKET_ERROR
    * Return value: int

Parameters::

    ** SOCKET s socket
    ** int level
    ** int optname
    *  const char *optval
    *  int optlen

Logging::

    b buffer (uintptr_t) optlen, optval


ioctlsocket
===========

Signature::

    * Is success: ret != SOCKET_ERROR
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  long cmd
    ** u_long *argp arg

Flags::

    cmd

Logging::

    x cmd cmd


closesocket
===========

Signature::

    * Is success: ret != SOCKET_ERROR
    * Return value: int

Parameters::

    ** SOCKET s socket


shutdown
========

Signature::

    * Is success: ret != SOCKET_ERROR
    * Return value: int

Parameters::

    ** SOCKET s socket
    ** int how


WSAAccept
=========

Signature::

    * Return value: SOCKET

Parameters::

    ** SOCKET s socket
    *  struct sockaddr *addr
    *  LPINT addrlen
    *  LPCONDITIONPROC lpfnCondition
    *  DWORD_PTR dwCallbackData

Middle::

    const char *ip = NULL; int port = 0;
    get_ip_port(addr, &ip, &port);

Logging::

    s addr ip
    i port port


WSARecv
=======

Signature::

    * Is success: ret > 0
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  LPWSABUF lpBuffers
    *  DWORD dwBufferCount
    *  LPDWORD lpNumberOfBytesRecvd
    *  LPDWORD lpFlags
    *  LPWSAOVERLAPPED lpOverlapped
    *  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine

Middle::

    uint8_t *buf = NULL; uintptr_t length = 0;
    wsabuf_get_buffer(dwBufferCount, lpBuffers, &buf, &length);

    if(lpNumberOfBytesRecvd != NULL && *lpNumberOfBytesRecvd < length) {
        length = *lpNumberOfBytesRecvd;
    }

Logging::

    u len lpBuffers[0].len
    !b buf length, buf
    u dwNumberOfBytesRecvd lpNumberOfBytesRecvd != NULL ? *lpNumberOfBytesRecvd : 0
    u dwFlags lpFlags != NULL ? *lpFlags : 0

Post::

    mem_free(buf);


WSARecvFrom
===========

Signature::

    * Is success: ret > 0
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  LPWSABUF lpBuffers
    *  DWORD dwBufferCount
    *  LPDWORD lpNumberOfBytesRecvd
    *  LPDWORD lpFlags
    *  struct sockaddr *lpFrom
    *  LPINT lpFromlen
    *  LPWSAOVERLAPPED lpOverlapped
    *  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine

Middle::

    const char *ip = NULL; int port = 0;
    get_ip_port(lpFrom, &ip, &port);

    uint8_t *buf = NULL; uintptr_t length = 0;
    wsabuf_get_buffer(dwBufferCount, lpBuffers, &buf, &length);

    if(lpNumberOfBytesRecvd != NULL && *lpNumberOfBytesRecvd < length) {
        length = *lpNumberOfBytesRecvd;
    }

Logging::

    s addr ip
    i port port
    u dwFlags lpFlags != NULL ? *lpFlags : 0
    u dwNumberOfBytesRecvd lpNumberOfBytesRecvd != NULL ? *lpNumberOfBytesRecvd : 0
    !b buff length, buf
    u len length

Post::

    mem_free(buf);


WSASend
=======

Signature::

    * Is success: ret > 0
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  LPWSABUF lpBuffers
    ** DWORD dwBufferCount dwBufferCount
    ** LPDWORD lpNumberOfBytesSent dwNumberOfBytesSent
    ** DWORD dwFlags dwFlags
    *  LPWSAOVERLAPPED lpOverlapped
    *  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine

Middle::

    uint8_t *buf = NULL; uintptr_t length = 0;
    wsabuf_get_buffer(dwBufferCount, lpBuffers, &buf, &length);

    if(lpNumberOfBytesSent != NULL && *lpNumberOfBytesSent < length) {
        length = *lpNumberOfBytesSent;
    }

Logging::

    !b buff length, buf
    u len length
    

Post::

    mem_free(buf);


WSASendTo
=========

Signature::

    * Is success: ret > 0
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  LPWSABUF lpBuffers
    ** DWORD dwBufferCount dwBufferCount
    ** LPDWORD lpNumberOfBytesSent dwNumberOfBytesSent
    ** DWORD dwFlags dwFlags
    *  const struct sockaddr *lpTo
    *  int iToLen
    *  LPWSAOVERLAPPED lpOverlapped
    *  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine

Pre::

    const char *ip = NULL; int port = 0;
    get_ip_port(lpTo, &ip, &port);

Middle::

    uint8_t *buf = NULL; uintptr_t length = 0;
    wsabuf_get_buffer(dwBufferCount, lpBuffers, &buf, &length);

    if(lpNumberOfBytesSent != NULL && *lpNumberOfBytesSent < length) {
        length = *lpNumberOfBytesSent;
    }

Logging::

    s addr ip
    i port port
    !b buff length, buf
    u len length

Post::

    mem_free(buf);


WSASocketA
==========

Signature::

    * Return value: SOCKET

Parameters::

    ** int af
    ** int type
    ** int protocol
    *  LPWSAPROTOCOL_INFO lpProtocolInfo
    *  GROUP g
    ** DWORD dwFlags flags

Interesting::

    i af
    i type
    i protocol
    i flags

Logging::

    i socket ret


WSASocketW
==========

Signature::

    * Return value: SOCKET

Parameters::

    ** int af af
    ** int type type
    ** int protocol protocol
    *  LPWSAPROTOCOL_INFO lpProtocolInfo
    *  GROUP g
    ** DWORD dwFlags dwFlags

Interesting::

    i af
    i type
    i protocol
    i dwFlags

Logging::

    i socket ret
    s szProtocol (lpProtocolInfo != NULL) ? lpProtocolInfo->szProtocol : "(null)"


WSAConnect
==========

Signature::

    * Is success: ret == 0
    * Return value: int

Parameters::

    ** SOCKET s socket
    *  const struct sockaddr *name
    *  int namelen
    *  LPWSABUF lpCallerData
    *  LPWSABUF lpCalleeData
    *  LPQOS lpSQOS
    *  LPQOS lpGQOS

Pre::

    const char *ip = NULL; int port = 0;
    get_ip_port(name, &ip, &port);

    // TODO Dump lpCallerData and lpCalleeData.

Logging::

    s addr ip
    i port port


ConnectEx
=========

Signature::

    * Prune: resolve
    * Return value: BOOL

Parameters::

    ** SOCKET s socket
    *  const struct sockaddr *name
    *  int namelen
    *  PVOID lpSendBuffer
    *  DWORD dwSendDataLength
    *  LPDWORD lpdwBytesSent
    *  LPOVERLAPPED lpOverlapped

Ensure::

    lpdwBytesSent

Pre::

    const char *ip = NULL; int port = 0;
    get_ip_port(name, &ip, &port);

Logging::

    s ip_address ip
    i port port
    b buffer (uintptr_t) *lpdwBytesSent, lpSendBuffer


TransmitFile
============

Signature::

    * Prune: resolve
    * Return value: BOOL

Parameters::

    ** SOCKET hSocket socket
    ** HANDLE hFile file_handle
    ** DWORD nNumberOfBytesToWrite
    ** DWORD nNumberOfBytesPerSend
    *  LPOVERLAPPED lpOverlapped
    *  LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers
    *  DWORD dwFlags
