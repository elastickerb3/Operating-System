#include <efi.h>

#include "../additional-src/Variables.hpp"
#include "../additional-src/Shutdown+Reboot.hpp"
#include "../additional-src/terminal.hpp"
#include "../additional-src/WorkingWithChars.hpp"
#include "../additional-src/input.hpp"
#include "../additional-src/color.hpp"
#include "../additional-src/command.hpp"

void uefi_print(EFI_SYSTEM_TABLE *SystemTable, const wchar_t* string) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16*)string);
}

EFI_STATUS SetBestGopMode() {
    UINT32 best = Gop->Mode->Mode; UINT64 bestPx = 0;
    for (UINT32 i = 0; i < Gop->Mode->MaxMode; i++) {
        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info; UINTN size;
        if (EFI_ERROR(Gop->QueryMode(Gop, i, &size, &info))) continue;
        if (info->PixelFormat != PixelBltOnly) {
            UINT64 px = (UINT64)info->HorizontalResolution * info->VerticalResolution;
            if (px > bestPx) { bestPx = px; best = i; }
        }
        gBS->FreePool(info);
    }
    return Gop->SetMode(Gop, best);
}

extern "C" EFI_STATUS kernel_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SysteTable) {
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

    if(!UseingQEMU){
        SetBestGopMode();    
    }

    clear(BG_COLOR);
    print(L"Willkommen zum Potatoe OS.\n", FALSE);

    while (TRUE) {
        CHAR16* command = input((wchar_t*)PATH, TRUE);
        
        if(command[0] == L'\0'){} else if(CommandInhalt(command, (CHAR16*)L"clear") || CommandInhalt(command, (CHAR16*)L"cls")){
            clear(BG_COLOR);
        }else if(CommandInhalt(command, (CHAR16*)L"shutdown")){
            shutdown();
        }else if(CommandInhalt(command, (CHAR16*)L"help")){
            print(L"help        -- diese hilfe liste\n", FALSE);
            print(L"reboot      -- pc neustart\n", FALSE);
            print(L"shutdown    -- pc herunterfahren\n", FALSE);
            print(L"cls/clear   -- console leeren\n", FALSE);
            print(L"color       -- Farbe von Hintergrund und schrift ändern\n", FALSE);
        }else if(CommandInhalt(command, (CHAR16*)L"color")){
            CHAR16* Font_color_param = parameter(command, (CHAR16*)L"color")[0];
            CHAR16* Backgound_color_param = parameter(command, (CHAR16*)L"color")[1];
            if (str_cmp(Font_color_param, (CHAR16*)L"/?") || str_cmp(Font_color_param, (CHAR16*)L"--help")) {
                print(L"color [Schriftfarbe in hex] [optional Hintergrund farbe]\n", FALSE);
                print(L"Farben:\n", FALSE);
                print(L"0:schwartz", FALSE);
                print(L"1:dunkelblau, 2:dunkelgrün\n", FALSE);
                print(L"3:tyrkis, 4:rot, 5:pink\n", FALSE);
                print(L"6:orange, 7:hellgrau\n", FALSE);
                print(L"8:dunkelgrau, 9:violett\n", FALSE);
                print(L"a:hellgrün, b:hellblau\n", FALSE);
                print(L"c:hellrot, d:lila\n", FALSE);
                print(L"e:gelb, f:weiß\n", FALSE);
            }else{
                if(BIOS_IndexIntoHex(Font_color_param) == BIOS_IndexIntoHex(Backgound_color_param)){
                    print(L"Schriftfarbe ist entweder nicht verfügbar oder beide Farben sind die Selben.\n", FALSE);
                    continue;
                }
                FONT_COLOR = BIOS_IndexIntoHex(Font_color_param);
                BG_COLOR = BIOS_IndexIntoHex(Backgound_color_param);
                clear(BG_COLOR);
            }
        }else if(CommandInhalt(command,(CHAR16*)L"reboot")){
            reboot();
        }else{
            STATIC CHAR16 text[InputMaxLength + 32];
            text[0] = L'\0';
            ConnectText(text, command);
            ConnectText(text, (CHAR16*)L" :Befehl nicht gefunden.\n");
            print((wchar_t*)text, FALSE);
        }
    }

    return EFI_SUCCESS;
}