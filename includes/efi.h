#pragma once

//UEFI固有のタグ？
#define EFIAPI
#define IN
#define OUT
#define OPTIONAL
#define NULL 0

//プロトコルのGUID
#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID \
  {0x9042a9de,0x23dc,0x4a38,\
  {0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}}

//UEFI固有の返り値
#define EFI_SUCCESS               0x0
#define EFI_BUFFER_TOO_SMALL      0x8000000000000005
#define EFI_INVALID_PARAMETER	  0x8000000000000002

//UEFI固有の型
typedef unsigned long long UINTN;
typedef short CHAR16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef void VOID;

typedef UINTN EFI_STATUS;
typedef struct {
  unsigned long  Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char  Data4[8];
} EFI_GUID;
typedef void* EFI_HANDLE;

typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

//テキストの出力に関わる構造体
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
  char buf1[8];  /*EFI_TEXT_RESET                         Reset;                   */
  //文字を出力する
  EFI_STATUS
  (EFIAPI *OutputString)(
    IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN CHAR16 *String
  );
  char buf2[8];  /*EFI_TEXT_TEST_STRING                   TestString;             */
  char buf3[8];  /*EFI_TEXT_QUERY_MODE                    QueryMode;           */
  char buf4[8];  /*EFI_TEXT_SET_MODE                      SetMode;               */
  char buf5[8];  /*EFI_TEXT_SET_ATTRIBUTE                 SetAttribute;          */
  //画面を初期化する
  EFI_STATUS
  (EFIAPI *ClearScreen) (
    IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
  );
  char buf7[8];  /*EFI_TEXT_SET_CURSOR_POSITION           SetCursorPosition;  */
  char buf8[8];  /*EFI_TEXT_ENABLE_CURSOR                 EnableCursor;        */
  char buf9[8];  /*SIMPLE_TEXT_OUTPUT_MODE                *Mode;                   */
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

//メモリマップ（1Page分）
typedef struct {
  UINT32 Type;
  EFI_PHYSICAL_ADDRESS PhysicalStart;
  EFI_VIRTUAL_ADDRESS VirtualStart;
  UINT64 NumberOfPages;
  UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;

//ページの種類
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

//ブートサービス
typedef struct {
  char buf1[24]; /* EFI_TABLE_HEADER Hdr; */

  char buf2[8]; /* EFI_RAISE_TPL RaiseTPL; */
  char buf3[8]; /* EFI_RESTORE_TPL RestoreTPL; */

  char buf4[8]; /*EFI_ALLOCATE_PAGES AllocatePages;*/
  char buf5[8]; /* EFI_FREE_PAGES FreePages; */
  //メモリマップを取得する関数
  EFI_STATUS
  (EFIAPI *GetMemoryMap) (
    IN OUT UINTN *MemoryMapSize,
    OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
    OUT UINTN *MapKey,
    OUT UINTN *DescriptorSize,
    OUT UINT32 *DescriptorVersion
  );
  //Bufferアドレスのメモリを確保する関数
  EFI_STATUS
  (EFIAPI *AllocatePool) (
    IN EFI_MEMORY_TYPE PoolType,
    IN UINTN Size,
    OUT VOID **Buffer
  );
  char buf6[160];
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
  EFI_IMAGE_UNLOAD UnloadImage; // EFI 1.0+*/
  //BootServicesを抜ける関数
  EFI_STATUS
  (EFIAPI *ExitBootServices) (
    IN EFI_HANDLE ImageHandle,
    IN UINTN MapKey
  );
  char buf7[80];
  /*
  // 
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
  EFI_LOCATE_HANDLE_BUFFER LocateHandleBuffer; // EFI 1.1+*/
  //GUIDからProtocolを取得する関数
  EFI_STATUS
  (EFIAPI *LocateProtocol) (
    IN EFI_GUID *Protocol,
    IN VOID *Registration OPTIONAL,
    OUT VOID **Interface
  );
  /*EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES
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

//システムテーブル
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

//フレームバッファ1ピクセルのフォーマット
typedef enum {
  PixelRedGreenBlueReserved8BitPerColor,
  PixelBlueGreenRedReserved8BitPerColor,
  PixelBitMask,
  PixelBltOnly,
  PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

//GOPのモードに関する情報
typedef struct {
  UINT32 MaxMode;
  UINT32 Mode;
  struct {
    UINT32 Version;
    UINT32 HorizontalResolution;
    UINT32 VerticalResolution;
    EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
    /* EFI_PIXEL_BITMASK PixelInformation;
    UINT32 PixelsPerScanLine; */
  } *Info;
  UINTN SizeOfInfo;
  EFI_PHYSICAL_ADDRESS FrameBufferBase;
  UINTN FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

//GOP
typedef struct {
  char buf1[8]; /* EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE QueryMode; */
  char buf2[8]; /* EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE SetMode; */
  char buf3[8]; /* EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT Blt; */
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

//変数や関数のプロトタイプ宣言
extern EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
extern void efi_init(EFI_SYSTEM_TABLE *);