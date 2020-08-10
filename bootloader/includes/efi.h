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

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID \
  {0x0964e5b22,0x6459,0x11d2,\
  {0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

 #define EFI_FILE_INFO_ID \
   { \
     0x9576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b } \
   }
 

//UEFI固有の返り値
#define EFI_SUCCESS               0x0
#define EFI_BUFFER_TOO_SMALL      0x8000000000000005
#define EFI_INVALID_PARAMETER	  0x8000000000000002

#define EFI_FILE_MODE_READ 0x0000000000000001
#define EFI_FILE_MODE_WRITE 0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000

//UEFI固有の型
typedef unsigned long long UINTN;
typedef unsigned char UINT8;
typedef short CHAR16;
typedef short INT16;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef void VOID;

typedef UINTN EFI_STATUS;
typedef struct {
  UINT32 Data1;
  UINT16 Data2;
  UINT16 Data3;
  UINT8  Data4[8];
} EFI_GUID;
typedef void* EFI_HANDLE;

typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

//テキストの出力に関わる構造体
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
  EFI_PHYSICAL_ADDRESS buf1;  /*EFI_TEXT_RESET                         Reset;                   */
  //文字を出力する
  EFI_STATUS
  (EFIAPI *OutputString)(
    IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN CHAR16 *String
  );
  EFI_PHYSICAL_ADDRESS buf2;  /*EFI_TEXT_TEST_STRING                   TestString;             */
  EFI_PHYSICAL_ADDRESS buf3;  /*EFI_TEXT_QUERY_MODE                    QueryMode;           */
  EFI_PHYSICAL_ADDRESS buf4;  /*EFI_TEXT_SET_MODE                      SetMode;               */
  EFI_PHYSICAL_ADDRESS buf5;  /*EFI_TEXT_SET_ATTRIBUTE                 SetAttribute;          */
  //画面を初期化する
  EFI_STATUS
  (EFIAPI *ClearScreen) (
    IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
  );
  EFI_PHYSICAL_ADDRESS buf7;  /*EFI_TEXT_SET_CURSOR_POSITION           SetCursorPosition;  */
  EFI_PHYSICAL_ADDRESS buf8;  /*EFI_TEXT_ENABLE_CURSOR                 EnableCursor;        */
  EFI_PHYSICAL_ADDRESS buf9;  /*SIMPLE_TEXT_OUTPUT_MODE                *Mode;                   */
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

//割り当ての種類
typedef enum {
  AllocateAnyPages,
  AllocateMaxAddress,
  AllocateAddress,
  MaxAllocateType
} EFI_ALLOCATE_TYPE;

//ブートサービス
typedef struct {
  char buf1[24]; /* EFI_TABLE_HEADER Hdr; */

  EFI_PHYSICAL_ADDRESS buf2; /* EFI_RAISE_TPL RaiseTPL; */
  EFI_PHYSICAL_ADDRESS buf3; /* EFI_RESTORE_TPL RestoreTPL; */
  EFI_STATUS
  (EFIAPI *AllocatePages) (
    IN EFI_ALLOCATE_TYPE Type,
    IN EFI_MEMORY_TYPE MemoryType,
    IN UINTN Pages,
    IN OUT EFI_PHYSICAL_ADDRESS *Memory
  );
  EFI_PHYSICAL_ADDRESS buf5; /* EFI_FREE_PAGES FreePages; */
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
  EFI_STATUS
  (EFIAPI *FreePool) (
    IN VOID *Buffer
  );
  EFI_PHYSICAL_ADDRESS buf6[19];
  /*//
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
  EFI_UNINSTALL_PROTOCOL_INTERFACE UninstallProtocolInterface; // EFI 1.0+*/
  /*EFI_STATUS
  (EFIAPI *HandleProtocol) (
    IN EFI_HANDLE Handle,
    IN EFI_GUID *Protocol,
    OUT VOID **Interface
  );*/
  /*VOID* Reserved; // EFI 1.0+
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
  EFI_PHYSICAL_ADDRESS buf7[10];
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
  EFI_PHYSICAL_ADDRESS buf8[5];
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
  //*/
  VOID
  (EFIAPI *CopyMem) (
    IN VOID *Destination,
    IN VOID *Source,
    IN UINTN Length
  );
  VOID
  (EFIAPI *SetMem) (
    IN VOID *Buffer,
    IN UINTN Size,
    IN UINT8 Value
  );
  /*EFI_CREATE_EVENT_EX CreateEventEx; // UEFI 2.0+*/
} EFI_BOOT_SERVICES;

//システムテーブル
typedef struct {
  char buf1[24];  /* EFI_TABLE_HEADER                              Hdr;                          */
  EFI_PHYSICAL_ADDRESS buf2;    /* CHAR16                                                *FirmwareVendor;     */
  UINT32 buf3;    /* UINT32                                                 FirmwareRevision;     */
  EFI_PHYSICAL_ADDRESS buf4;    /* EFI_HANDLE                                         ConsoleInHandle;       */
  EFI_PHYSICAL_ADDRESS buf5;    /* EFI_SIMPLE_TEXT_INPUT_PROTOCOL    *ConIn;                     */
  EFI_PHYSICAL_ADDRESS buf6;    /* EFI_HANDLE                                         ConsoleOutHandle;     */
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
  EFI_PHYSICAL_ADDRESS buf7;    /* EFI_HANDLE                                         StandardErrorHandle; */
  EFI_PHYSICAL_ADDRESS buf8;    /* EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;                     */
  EFI_PHYSICAL_ADDRESS buf9;    /* EFI_RUNTIME_SERVICES                       *RuntimeServices;      */
  EFI_BOOT_SERVICES                            *BootServices;
  UINTN buf11;  /* UINTN                                                  NumberOfTableEntries;*/
  EFI_PHYSICAL_ADDRESS buf12;  /* EFI_CONFIGURATION_TABLE                 *ConfigurationTable;    */
} EFI_SYSTEM_TABLE;

//フレームバッファ1ピクセルのフォーマット
typedef enum {
  PixelRedGreenBlueReserved8BitPerColor,
  PixelBlueGreenRedReserved8BitPerColor,
  PixelBitMask,
  PixelBltOnly,
  PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

//1ピクセルのデータ
typedef struct {
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

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
  EFI_PHYSICAL_ADDRESS buf1; /* EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE QueryMode; */
  EFI_PHYSICAL_ADDRESS buf2; /* EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE SetMode; */
  EFI_PHYSICAL_ADDRESS buf3; /* EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT Blt; */
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

//EFP
typedef struct _EFI_FILE_PROTOCOL {
  UINT64 Revision;
  EFI_STATUS
  (EFIAPI *Open) (
    IN struct _EFI_FILE_PROTOCOL *This,
    OUT struct _EFI_FILE_PROTOCOL **NewHandle,
    IN CHAR16 *FileName,
    IN UINT64 OpenMode,
    IN UINT64 Attributes
  );
  EFI_STATUS
  (EFIAPI *Close) (
    IN struct _EFI_FILE_PROTOCOL *This
  );
  EFI_STATUS
  (EFIAPI *Delete) (
    IN struct _EFI_FILE_PROTOCOL *This
  );
  EFI_STATUS
  (EFIAPI *Read) (
    IN struct _EFI_FILE_PROTOCOL *This,
    IN OUT UINTN *BufferSize,
    OUT VOID *Buffer
  );
  EFI_STATUS
  (EFIAPI *Write) (
    IN struct _EFI_FILE_PROTOCOL *This,
    IN OUT UINTN *BufferSize,
    IN VOID *Buffer
  );
  char buf1[8]; //EFI_FILE_GET_POSITION GetPosition;
  char buf2[8]; //EFI_FILE_SET_POSITION SetPosition;
  EFI_STATUS
  (EFIAPI *GetInfo) (
    IN struct _EFI_FILE_PROTOCOL *This,
    IN EFI_GUID *InformationType,
    IN OUT UINTN *BufferSize,
    OUT VOID *Buffer
  );
  char buf4[8]; //EFI_FILE_SET_INFO SetInfo;
  EFI_STATUS
  (EFIAPI *Flush) (
    IN struct _EFI_FILE_PROTOCOL *This
  );
  char buf5[8]; //EFI_FILE_OPEN_EX OpenEx; // Added for revision 2
  char buf6[8]; //EFI_FILE_READ_EX ReadEx; // Added for revision 2
  char buf7[8]; //EFI_FILE_WRITE_EX WriteEx; // Added for revision 2
  char buf8[8]; //EFI_FILE_FLUSH_EX FlushEx; // Added for revision 2
} EFI_FILE_PROTOCOL;

//ESFSP
typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
  UINT64 Revision;
  EFI_STATUS
  (EFIAPI *OpenVolume) (
    IN struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
    OUT EFI_FILE_PROTOCOL **Root
  );
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

//時間情報
typedef struct {
 UINT16 Year; // 1900 – 9999
 UINT8 Month; // 1 – 12
 UINT8 Day; // 1 – 31
 UINT8 Hour; // 0 – 23
 UINT8 Minute; // 0 – 59
 UINT8 Second; // 0 – 59
 UINT8 Pad1;
 UINT32 Nanosecond; // 0 – 999,999,999
 INT16 TimeZone; // -1440 to 1440 or 2047
 UINT8 Daylight;
 UINT8 Pad2;
} EFI_TIME;

//ファイルの情報
typedef struct {
  UINT64 Size;
  UINT64 FileSize;
  UINT64 PhysicalSize;
  EFI_TIME CreateTime;
  EFI_TIME LastAccessTime;
  EFI_TIME ModificationTime;
  UINT64 Attribute;
  CHAR16 FileName[];
} EFI_FILE_INFO;

//変数や関数のプロトタイプ宣言
extern EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
extern EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *ESFSP;
extern void efi_init(EFI_SYSTEM_TABLE *, EFI_HANDLE);

struct fb {
	unsigned long long base;
	unsigned long long size;
	unsigned int hr;
	unsigned int vr;
};