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

#ifndef MONITOR_MONITOR_H
#define MONITOR_MONITOR_H

#include <stdint.h>
#include <windows.h>

extern char* iret();

//基本类型定义
typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned __int64    UINT64, *PUINT64;
//基本类型定义完毕



typedef INT64 int64;
typedef UINT64 uint64;
typedef UINT32 cardinal;
typedef INT32 integer;
typedef UINT32 dword;
typedef UINT32 uint;
typedef UINT16 word;
typedef UINT8 byte;

#ifdef _M_X64
typedef UINT64 nativeuint;
typedef UINT64 pointer;
typedef INT64 nativeint;
#endif // _M_X64
#ifdef _M_IX86 
typedef UINT32 nativeuint;
typedef UINT32 pointer;
typedef INT32 nativeint;
#endif // _M_IX86
typedef pointer pchar;
typedef pointer THandle;
typedef pchar string;
//#define export __attribute__((dllexport))
//#define export extern "C" __declspec(dllexport)
#define export __declspec(dllexport)
//#define default EMIT WARNING C4005
//#define _exportdll __attribute__((visibility("default")))


void monitor_init(HMODULE module_handle);
void monitor_hook(const char *library, void *module_handle);
void monitor_unhook(const char *library, void *module_handle);

extern uint32_t g_monitor_mode;

#endif
