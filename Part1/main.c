#define EFIAPI
#define IN
typedef unsigned long long UINTN;
typedef UINTN EFI_STATUS;
typedef short CHAR16;
typedef void * EFI_HANDLE;

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_STRING)(
  IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN CHAR16 *String
);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
  char buf1[8];  /*EFI_TEXT_RESET                              Reset;                   */
  EFI_TEXT_STRING OutputString;
  char buf2[8];  /*EFI_TEXT_TEST_STRING                   TestString;             */
  char buf3[8];  /*EFI_TEXT_QUERY_MODE                   QueryMode;           */
  char buf4[8];  /*EFI_TEXT_SET_MODE                        SetMode;               */
  char buf5[8];  /*EFI_TEXT_SET_ATTRIBUTE                SetAttribute;          */
  char buf6[8];  /*EFI_TEXT_CLEAR_SCREEN                 ClearScreen;          */
  char buf7[8];  /*EFI_TEXT_SET_CURSOR_POSITION    SetCursorPosition;  */
  char buf8[8];  /*EFI_TEXT_ENABLE_CURSOR               EnableCursor;        */
  char buf9[8];  /*SIMPLE_TEXT_OUTPUT_MODE           *Mode;                   */
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

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
  char buf10[8];  /* EFI_BOOT_SERVICES                            *BootServices;            */
  char buf11[8];  /* UINTN                                                  NumberOfTableEntries;*/
  char buf12[8];  /* EFI_CONFIGURATION_TABLE                 *ConfigurationTable;    */
} EFI_SYSTEM_TABLE;

EFI_STATUS
efi_main (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
) {
  SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, world!");
  while (1);
}