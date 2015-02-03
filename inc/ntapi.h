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

#ifndef MONITOR_NTAPI_H__
#define MONITOR_NTAPI_H__

#include <stdint.h>
#include <windows.h>
#include <wincrypt.h>

typedef LONG NTSTATUS;
typedef void *PIO_APC_ROUTINE;

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR  Buffer;
} ANSI_STRING, *PANSI_STRING;

typedef struct _LSA_UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID    Pointer;
    } _;
    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG           Length;
    HANDLE          RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG           Attributes;
    PVOID           SecurityDescriptor;
    PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef void *HINTERNET;

typedef struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    size_t           ai_addrlen;
    char             *ai_canonname;
    struct sockaddr  *ai_addr;
    struct addrinfo  *ai_next;
} ADDRINFOA, *PADDRINFOA;

typedef struct addrinfoW {
    int               ai_flags;
    int               ai_family;
    int               ai_socktype;
    int               ai_protocol;
    size_t            ai_addrlen;
    PWSTR             ai_canonname;
    struct sockaddr  *ai_addr;
    struct addrinfoW *ai_next;
} ADDRINFOW, *PADDRINFOW;

typedef enum _KEY_INFORMATION_CLASS {
    KeyBasicInformation            = 0,
    KeyNodeInformation             = 1,
    KeyFullInformation             = 2,
    KeyNameInformation             = 3,
    KeyCachedInformation           = 4,
    KeyFlagsInformation            = 5,
    KeyVirtualizationInformation   = 6,
    KeyHandleTagsInformation       = 7,
    MaxKeyInfoClass                = 8
} KEY_INFORMATION_CLASS;

typedef enum _KEY_VALUE_INFORMATION_CLASS {
    KeyValueBasicInformation            = 0,
    KeyValueFullInformation             = 1,
    KeyValuePartialInformation          = 2,
    KeyValueFullInformationAlign64      = 3,
    KeyValuePartialInformationAlign64   = 4,
    MaxKeyValueInfoClass                = 5
} KEY_VALUE_INFORMATION_CLASS;

typedef struct _KEY_VALUE_BASIC_INFORMATION {
    ULONG TitleIndex;
    ULONG Type;
    ULONG NameLength;
    WCHAR Name[1];
} KEY_VALUE_BASIC_INFORMATION, *PKEY_VALUE_BASIC_INFORMATION;

typedef struct _KEY_VALUE_FULL_INFORMATION {
    ULONG TitleIndex;
    ULONG Type;
    ULONG DataOffset;
    ULONG DataLength;
    ULONG NameLength;
    WCHAR Name[1];
} KEY_VALUE_FULL_INFORMATION, *PKEY_VALUE_FULL_INFORMATION;

typedef struct _KEY_VALUE_PARTIAL_INFORMATION {
    ULONG TitleIndex;
    ULONG Type;
    ULONG DataLength;
    UCHAR Data[1];
} KEY_VALUE_PARTIAL_INFORMATION, *PKEY_VALUE_PARTIAL_INFORMATION;

typedef struct _KEY_VALUE_ENTRY {
    PUNICODE_STRING    ValueName;
    ULONG        DataLength;
    ULONG        DataOffset;
    ULONG        Type;
} KEY_VALUE_ENTRY, *PKEY_VALUE_ENTRY;

typedef struct _PROCESS_BASIC_INFORMATION {
    PVOID Reserved1;
    void *PebBaseAddress;
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    PVOID InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;

typedef struct _CLIENT_ID {
    PVOID UniqueProcess;
    PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef ULONG_PTR KAFFINITY;
typedef LONG KPRIORITY;

typedef struct _THREAD_BASIC_INFORMATION {
    NTSTATUS ExitStatus;
    PVOID TebBaseAddress;
    CLIENT_ID ClientId;
    KAFFINITY AffinityMask;
    KPRIORITY Priority;
    KPRIORITY BasePriority;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

typedef struct _INITIAL_TEB {
    PVOID StackBase;
    PVOID StackLimit;
    PVOID StackCommit;
    PVOID StackCommitMax;
    PVOID StackReserved;
} INITIAL_TEB, *PINITIAL_TEB;

typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation = 1,
    FileFullDirectoryInformation,
    FileBothDirectoryInformation,
    FileBasicInformation,
    FileStandardInformation,
    FileInternalInformation,
    FileEaInformation,
    FileAccessInformation,
    FileNameInformation,
    FileRenameInformation,
    FileLinkInformation,
    FileNamesInformation,
    FileDispositionInformation,
    FilePositionInformation,
    FileFullEaInformation,
    FileModeInformation,
    FileAlignmentInformation,
    FileAllInformation,
    FileAllocationInformation,
    FileEndOfFileInformation,
    FileAlternateNameInformation,
    FileStreamInformation,
    FilePipeInformation,
    FilePipeLocalInformation,
    FilePipeRemoteInformation,
    FileMailslotQueryInformation,
    FileMailslotSetInformation,
    FileCompressionInformation,
    FileObjectIdInformation,
    FileCompletionInformation,
    FileMoveClusterInformation,
    FileQuotaInformation,
    FileReparsePointInformation,
    FileNetworkOpenInformation,
    FileAttributeTagInformation,
    FileTrackingInformation,
    FileIdBothDirectoryInformation,
    FileIdFullDirectoryInformation,
    FileValidDataLengthInformation,
    FileShortNameInformation,
    FileIoCompletionNotificationInformation,
    FileIoStatusBlockRangeInformation,
    FileIoPriorityHintInformation,
    FileSfioReserveInformation,
    FileSfioVolumeInformation,
    FileHardLinkInformation,
    FileProcessIdsUsingFileInformation,
    FileNormalizedNameInformation,
    FileNetworkPhysicalNameInformation,
    FileIdGlobalTxDirectoryInformation,
    FileIsRemoteDeviceInformation,
    FileAttributeCacheInformation,
    FileNumaNodeInformation,
    FileStandardLinkInformation,
    FileRemoteProtocolInformation,
    FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

#define STATUS_ACCESS_DENIED ((NTSTATUS) 0xc0000022)

typedef struct _FILE_BASIC_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    ULONG FileAttributes;
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

typedef struct _RTL_DRIVE_LETTER_CURDIR {
    USHORT Flags;
    USHORT Length;
    ULONG TimeStamp;
    UNICODE_STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    ULONG                   MaximumLength;
    ULONG                   Length;
    ULONG                   Flags;
    ULONG                   DebugFlags;
    PVOID                   ConsoleHandle;
    ULONG                   ConsoleFlags;
    HANDLE                  StdInputHandle;
    HANDLE                  StdOutputHandle;
    HANDLE                  StdErrorHandle;
    UNICODE_STRING          CurrentDirectoryPath;
    HANDLE                  CurrentDirectoryHandle;
    UNICODE_STRING          DllPath;
    UNICODE_STRING          ImagePathName;
    UNICODE_STRING          CommandLine;
    PVOID                   Environment;
    ULONG                   StartingPositionLeft;
    ULONG                   StartingPositionTop;
    ULONG                   Width;
    ULONG                   Height;
    ULONG                   CharWidth;
    ULONG                   CharHeight;
    ULONG                   ConsoleTextAttributes;
    ULONG                   WindowFlags;
    ULONG                   ShowWindowFlags;
    UNICODE_STRING          WindowTitle;
    UNICODE_STRING          DesktopName;
    UNICODE_STRING          ShellInfo;
    UNICODE_STRING          RuntimeData;
    RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef void *PPS_CREATE_INFO, *PPS_ATTRIBUTE_LIST;

typedef void *PVOID, **PPVOID;

typedef struct _PEB_LDR_DATA {
    ULONG Length;
    BOOLEAN Initialized;
    PVOID SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _LDR_MODULE {
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID BaseAddress;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    SHORT LoadCount;
    SHORT TlsIndex;
    LIST_ENTRY HashTableEntry;
    ULONG TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;

typedef struct _PEB {
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    BOOLEAN Spare;
    HANDLE  Mutant;
    PVOID   ImageBaseAddress;
    PPEB_LDR_DATA LoaderData;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    PVOID   SubSystemData;
    PVOID   ProcessHeap;
    PVOID   FastPebLock;
    void   *FastPebLockRoutine;
    void   *FastPebUnlockRoutine;
    ULONG   EnvironmentUpdateCount;
    PPVOID  KernelCallbackTable;
    PVOID   EventLogSection;
    PVOID   EventLog;
    void   *FreeList;
    ULONG   TlsExpansionCounter;
    PVOID   TlsBitmap;
    ULONG   TlsBitmapBits[0x2];
    PVOID   ReadOnlySharedMemoryBase;
    PVOID   ReadOnlySharedMemoryHeap;
    PPVOID  ReadOnlyStaticServerData;
    PVOID   AnsiCodePageData;
    PVOID   OemCodePageData;
    PVOID   UnicodeCaseTableData;
    ULONG   NumberOfProcessors;
    ULONG   NtGlobalFlag;
    BYTE    Spare2[0x4];
    LARGE_INTEGER CriticalSectionTimeout;
    ULONG   HeapSegmentReserve;
    ULONG   HeapSegmentCommit;
    ULONG   HeapDeCommitTotalFreeThreshold;
    ULONG   HeapDeCommitFreeBlockThreshold;
    ULONG   NumberOfHeaps;
    ULONG   MaximumNumberOfHeaps;
    PPVOID *ProcessHeaps;
    PVOID   GdiSharedHandleTable;
    PVOID   ProcessStarterHelper;
    PVOID   GdiDCAttributeList;
    PVOID   LoaderLock;
    ULONG   OSMajorVersion;
    ULONG   OSMinorVersion;
    ULONG   OSBuildNumber;
    ULONG   OSPlatformId;
    ULONG   ImageSubSystem;
    ULONG   ImageSubSystemMajorVersion;
    ULONG   ImageSubSystemMinorVersion;
    ULONG   GdiHandleBuffer[0x22];
    ULONG   PostProcessInitRoutine;
    ULONG   TlsExpansionBitmap;
    BYTE    TlsExpansionBitmapBits[0x80];
    ULONG   SessionId;
} PEB, *PPEB;

#if __x86_64__

#define TLS_TEB 0x30
#define TLS_PEB 0x60

static inline uintptr_t readtls(uint32_t index)
{
    uintptr_t ret;
    __asm__ volatile("movq %%gs:(%1), %0" : "=r" (ret) : "r" (index));
    return ret;
}

static inline void writetls(uint32_t index, uintptr_t value)
{
    __asm__ volatile("movq %0, %%gs:(%1)" :: "r" (value), "r" (index));
}

#else

#define TLS_TEB 0x18
#define TLS_PEB 0x30

static inline uintptr_t readtls(uint32_t index)
{
    uintptr_t ret;
    __asm__ volatile("movl %%fs:(%1), %0" : "=r" (ret) : "r" (index));
    return ret;
}

static inline void writetls(uint32_t index, uintptr_t value)
{
    __asm__ volatile("movl %0, %%fs:(%1)" :: "r" (value), "r" (index));
}

#endif

typedef struct _SECTION_IMAGE_INFORMATION {
    PVOID               TransferAddress;
    uint32_t            ZeroBits;
    uint8_t             _PADDING0_[0x4];
    uint64_t            MaximumStackSize;
    uint64_t            CommittedStackSize;
    uint32_t            SubSystemType;
    union {
        struct {
            uint16_t    SubSystemMinorVersion;
            uint16_t    SubSystemMajorVersion;
        } _;
        uint32_t        SubSystemVersion;
    } _;
    uint32_t            GpValue;
    uint16_t            ImageCharacteristics;
    uint16_t            DllCharacteristics;
    uint16_t            Machine;
    uint8_t             ImageContainsCode;
    union {
        uint8_t         ImageFlags;
        struct {
            uint8_t     ComPlusNativeReady : 1;
            uint8_t     ComPlusILOnly : 1;
            uint8_t     ImageDynamicallyRelocated : 1;
            uint8_t     ImageMappedFlat : 1;
            uint8_t     Reserved : 4;
        } _;
    } __;
    uint32_t            LoaderFlags;
    uint32_t            ImageFileSize;
    uint32_t            CheckSum;
} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;

typedef struct _RTL_USER_PROCESS_INFORMATION {
    ULONG Size;
    HANDLE ProcessHandle;
    HANDLE ThreadHandle;
    CLIENT_ID ClientId;
    SECTION_IMAGE_INFORMATION ImageInformation;
} RTL_USER_PROCESS_INFORMATION, *PRTL_USER_PROCESS_INFORMATION;

typedef struct _FILE_NAME_INFORMATION {
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

typedef enum  {
    FileFsVolumeInformation       = 1,
    FileFsLabelInformation        = 2,
    FileFsSizeInformation         = 3,
    FileFsDeviceInformation       = 4,
    FileFsAttributeInformation    = 5,
    FileFsControlInformation      = 6,
    FileFsFullSizeInformation     = 7,
    FileFsObjectIdInformation     = 8,
    FileFsDriverPathInformation   = 9,
    FileFsVolumeFlagsInformation  = 10,
    FileFsSectorSizeInformation   = 11
} FS_INFORMATION_CLASS;

typedef struct _FILE_FS_VOLUME_INFORMATION {
    LARGE_INTEGER VolumeCreationTime;
    ULONG         VolumeSerialNumber;
    ULONG         VolumeLabelLength;
    BOOLEAN       SupportsObjects;
    WCHAR         VolumeLabel[1];
} FILE_FS_VOLUME_INFORMATION, *PFILE_FS_VOLUME_INFORMATION;

typedef enum _FILE_INFO_BY_HANDLE_CLASS {
    FileBasicInfo                   = 0,
    FileStandardInfo                = 1,
    FileNameInfo                    = 2,
    FileRenameInfo                  = 3,
    FileDispositionInfo             = 4,
    FileAllocationInfo              = 5,
    FileEndOfFileInfo               = 6,
    FileStreamInfo                  = 7,
    FileCompressionInfo             = 8,
    FileAttributeTagInfo            = 9,
    FileIdBothDirectoryInfo         = 10,
    FileIdBothDirectoryRestartInfo  = 11,
    FileIoPriorityHintInfo          = 12,
    FileRemoteProtocolInfo          = 13,
    FileFullDirectoryInfo           = 14,
    FileFullDirectoryRestartInfo    = 15,
    FileStorageInfo                 = 16,
    FileAlignmentInfo               = 17,
    FileIdInfo                      = 18,
    FileIdExtdDirectoryInfo         = 19,
    FileIdExtdDirectoryRestartInfo  = 20,
    MaximumFileInfoByHandlesClass
} FILE_INFO_BY_HANDLE_CLASS, *PFILE_INFO_BY_HANDLE_CLASS;

typedef struct _KEY_NAME_INFORMATION {
    ULONG NameLength;
    WCHAR Name[1];
} KEY_NAME_INFORMATION, *PKEY_NAME_INFORMATION;

typedef enum _OBJECT_INFORMATION_CLASS {
    ObjectBasicInformation,
    ObjectNameInformation,
    ObjectTypeInformation,
    ObjectAllInformation,
    ObjectDataInformation
} OBJECT_INFORMATION_CLASS;

typedef struct _OBJECT_NAME_INFORMATION {
    UNICODE_STRING Name;
    WCHAR NameBuffer[1];
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;

typedef int EXTENDED_NAME_FORMAT;

static inline UNICODE_STRING *unistr_from_objattr(OBJECT_ATTRIBUTES *obj)
{
    return obj != NULL ? obj->ObjectName : NULL;
}

#define InitializeObjectAttributes(p, n, a, r, s) { \
    (p)->Length = sizeof(OBJECT_ATTRIBUTES); \
    (p)->RootDirectory = r;                  \
    (p)->Attributes = a;                     \
    (p)->ObjectName = n;                     \
    (p)->SecurityDescriptor = s;             \
    (p)->SecurityQualityOfService = NULL;    \
}

#define ProcessBasicInformation 0
#define ThreadBasicInformation 0

#define MAX_PATH_W 0x7fff

#endif
