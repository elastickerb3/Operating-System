#include <efi.h>
#include <efiapi.h>
#include <efibind.h>
#include <eficon.h>
#include <efidef.h>
#include <efierr.h>
#include <efinet.h>
#include <efiprot.h>
#include <efilib.h>

#include "../additional-src/Variables.hpp"
#include "../additional-src/terminal.hpp"

uint64_t entry_point_address = 0x100000; 

typedef void __attribute__((sysv_abi)) (*KernelEntryPoint)(EFI_HANDLE, EFI_SYSTEM_TABLE*);

void uefi_print(EFI_SYSTEM_TABLE *SystemTable, const wchar_t* string) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16*)string);
}

extern "C" EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SysteTable) {
    SystemTable = SysteTable;
    (void)ImageHandle;
    
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_STATUS status;

    gBS = SystemTable->BootServices;

    status = gBS->LocateProtocol(&gopGuid, nullptr, (void**)&Gop);

    ResetVariables();

    UINTN buffer_size = (UINTN)Gop->Mode->Info->HorizontalResolution * Gop->Mode->Info->VerticalResolution * sizeof(UINT32);
    status = gBS->AllocatePool(EfiBootServicesData, buffer_size, (VOID**)&ShadowFrameBuffer);
    if (EFI_ERROR(status)) {
        ShadowFrameBuffer = nullptr; 
    } else {
        UINTN total_pixels = (UINTN)Gop->Mode->Info->HorizontalResolution * Gop->Mode->Info->VerticalResolution;
        for (UINTN i = 0; i < total_pixels; i++) {
            ShadowFrameBuffer[i] = BG_COLOR; 
        }
    }


    if (EFI_ERROR(status)) {
        uefi_print(SystemTable, L"Fehler: GOP konnte nicht gefunden werden!\r\n");
        return status;
    }

    clear(BG_COLOR);
    print(L"Booted in den kernel ...\n", FALSE);

    KernelEntryPoint kernel_entry = (KernelEntryPoint)entry_point_address;
    kernel_entry(ImageHandle, SysteTable);


    return EFI_SUCCESS;
}