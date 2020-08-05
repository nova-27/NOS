#pragma once

#define EFIAPI
#define IN
#define OUT

typedef unsigned long long UINTN;
typedef UINTN EFI_STATUS;

typedef short CHAR16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef void VOID;

typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

typedef void * EFI_HANDLE;

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_STRING)(
  IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN CHAR16 *String
);

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_CLEAR_SCREEN) (
  IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
  char buf1[8];  /*EFI_TEXT_RESET                              Reset;                   */
  EFI_TEXT_STRING  OutputString;
  char buf2[8];  /*EFI_TEXT_TEST_STRING                   TestString;             */
  char buf3[8];  /*EFI_TEXT_QUERY_MODE                   QueryMode;           */
  char buf4[8];  /*EFI_TEXT_SET_MODE                        SetMode;               */
  char buf5[8];  /*EFI_TEXT_SET_ATTRIBUTE                SetAttribute;          */
  EFI_TEXT_CLEAR_SCREEN  ClearScreen;
  char buf7[8];  /*EFI_TEXT_SET_CURSOR_POSITION    SetCursorPosition;  */
  char buf8[8];  /*EFI_TEXT_ENABLE_CURSOR               EnableCursor;        */
  char buf9[8];  /*SIMPLE_TEXT_OUTPUT_MODE           *Mode;                   */
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct {
 UINT32 Type;
 EFI_PHYSICAL_ADDRESS PhysicalStart;
 EFI_VIRTUAL_ADDRESS VirtualStart;
 UINT64 NumberOfPages;
 UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef
EFI_STATUS
(EFIAPI *EFI_GET_MEMORY_MAP) (
 IN OUT UINTN *MemoryMapSize,
 OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
 OUT UINTN *MapKey,
 OUT UINTN *DescriptorSize,
 OUT UINT32 *DescriptorVersion
);

 typedef enum {
 EfiReservedMemoryType,
 EfiLoaderCode,
 EfiLoaderData,
 EfiBootServicesCode,
 EfiBootServicesData,
 EfiRuntimeServicesCode,
 EfiRuntimeServicesData,
 EfiConventionalMemory,
 EfiUnusableMemory,
 EfiACPIReclaimMemory,
 EfiACPIMemoryNVS,
 EfiMemoryMappedIO,
 EfiMemoryMappedIOPortSpace,
 EfiPalCode,
 EfiPersistentMemory,
 EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef
EFI_STATUS
(EFIAPI *EFI_ALLOCATE_POOL) (
 IN EFI_MEMORY_TYPE PoolType,
 IN UINTN Size,
 OUT VOID **Buffer
 );

typedef struct {
 char buf1[24]; /* EFI_TABLE_HEADER Hdr; */

 char buf2[8]; /* EFI_RAISE_TPL RaiseTPL; */
 char buf3[8]; /* EFI_RESTORE_TPL RestoreTPL; */

 char buf4[8]; /*EFI_ALLOCATE_PAGES AllocatePages;*/
 char buf5[8]; /* EFI_FREE_PAGES FreePages; */
 EFI_GET_MEMORY_MAP GetMemoryMap;
 EFI_ALLOCATE_POOL AllocatePool;
 /*EFI_FREE_POOL FreePool; // EFI 1.0+
 //
 // Event & Timer Services
 //
 EFI_CREATE_EVENT CreateEvent; // EFI 1.0+
EFI_SET_TIMER SetTimer; // EFI 1.0+
EFI_WAIT_FOR_EVENT WaitForEvent; // EFI 1.0+
 EFI_SIGNAL_EVENT SignalEvent; // EFI 1.0+
 EFI_CLOSE_EVENT CloseEvent; // EFI 1.0+
 EFI_CHECK_EVENT CheckEvent; // EFI 1.0+
//
 // Protocol Handler Services
 //
EFI_INSTALL_PROTOCOL_INTERFACE InstallProtocolInterface; // EFI 1.0+
EFI_REINSTALL_PROTOCOL_INTERFACE ReinstallProtocolInterface; // EFI 1.0+
EFI_UNINSTALL_PROTOCOL_INTERFACE UninstallProtocolInterface; // EFI 1.0+
 EFI_HANDLE_PROTOCOL HandleProtocol; // EFI 1.0+
VOID* Reserved; // EFI 1.0+
EFI_REGISTER_PROTOCOL_NOTIFY RegisterProtocolNotify; // EFI 1.0+
EFI_LOCATE_HANDLE LocateHandle; // EFI 1.0+
 EFI_LOCATE_DEVICE_PATH LocateDevicePath; // EFI 1.0+
 EFI_INSTALL_CONFIGURATION_TABLE InstallConfigurationTable; // EFI 1.0+
 //
 // Image Services
 //
 EFI_IMAGE_LOAD LoadImage; // EFI 1.0+
 EFI_IMAGE_START StartImage; // EFI 1.0+
 EFI_EXIT Exit; // EFI 1.0+
 EFI_IMAGE_UNLOAD UnloadImage; // EFI 1.0+
 EFI_EXIT_BOOT_SERVICES ExitBootServices; // EFI 1.0+
 //
UEFI Specification, Version 2.8 Errata B EFI System Table
UEFI Forum, Inc. May 2020 95
 // Miscellaneous Services
 //
 EFI_GET_NEXT_MONOTONIC_COUNT GetNextMonotonicCount; // EFI 1.0+
EFI_STALL Stall; // EFI 1.0+
EFI_SET_WATCHDOG_TIMER SetWatchdogTimer; // EFI 1.0+
//
 // DriverSupport Services
 //
 EFI_CONNECT_CONTROLLER ConnectController; // EFI 1.1
 EFI_DISCONNECT_CONTROLLER DisconnectController;// EFI 1.1+
 //
 // Open and Close Protocol Services
 //
EFI_OPEN_PROTOCOL OpenProtocol; // EFI 1.1+
 EFI_CLOSE_PROTOCOL CloseProtocol; // EFI 1.1+
 EFI_OPEN_PROTOCOL_INFORMATION OpenProtocolInformation; // EFI 1.1+
 //
 // Library Services
 //
 EFI_PROTOCOLS_PER_HANDLE ProtocolsPerHandle; // EFI 1.1+
 EFI_LOCATE_HANDLE_BUFFER LocateHandleBuffer; // EFI 1.1+
EFI_LOCATE_PROTOCOL LocateProtocol; // EFI 1.1+
EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES
 InstallMultipleProtocolInterfaces; // EFI 1.1+
EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES
 UninstallMultipleProtocolInterfaces; // EFI 1.1+
//
 // 32-bit CRC Services
 //
 EFI_CALCULATE_CRC32 CalculateCrc32; // EFI 1.1+
 //
 // Miscellaneous Services
//
 EFI_COPY_MEM CopyMem; // EFI 1.1+
 EFI_SET_MEM SetMem; // EFI 1.1+
 EFI_CREATE_EVENT_EX CreateEventEx; // UEFI 2.0+*/
} EFI_BOOT_SERVICES;

typedef struct {
  char buf1[24];  /* EFI_TABLE_HEADER                              Hdr;                          */
  char buf2[8];    /* CHAR16                                                *FirmwareVendor;     */
  char buf3[4];    /* UINT32                                                 FirmwareRevision;     */
  char buf4[8];    /* EFI_HANDLE                                         ConsoleInHandle;       */
  char buf5[8];    /* EFI_SIMPLE_TEXT_INPUT_PROTOCOL    *ConIn;                     */
  char buf6[8];    /* EFI_HANDLE                                         ConsoleOutHandle;     */
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
  char buf7[8];    /* EFI_HANDLE                                         StandardErrorHandle; */
  char buf8[8];    /* EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;                     */
  char buf9[8];    /* EFI_RUNTIME_SERVICES                       *RuntimeServices;      */
  EFI_BOOT_SERVICES                            *BootServices;
  char buf11[8];  /* UINTN                                                  NumberOfTableEntries;*/
  char buf12[8];  /* EFI_CONFIGURATION_TABLE                 *ConfigurationTable;    */
} EFI_SYSTEM_TABLE;