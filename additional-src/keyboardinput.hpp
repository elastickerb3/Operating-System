#pragma once

#include <efi.h>
#include <efiapi.h>

#include "./Variables.hpp"

CHAR16 GetGermanCharacter(EFI_INPUT_KEY Key) {
    if (Key.ScanCode != 0) {
        return L'\0'; 
    }

    for (int i = 0; i < 162; i++) {
        if (ScanCodeLookUpTableDE[i].SearchScanCode == 0 && 
            ScanCodeLookUpTableDE[i].SearchUnicode == Key.UnicodeChar) {
            return ScanCodeLookUpTableDE[i].RealGermanChar;
        }
    }

    return (Key.UnicodeChar);
}

CHAR16* key_board_input(EFI_SYSTEM_TABLE *SystemTable){
    EFI_INPUT_KEY Key;
    EFI_STATUS status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
    if(status == EFI_SUCCESS){
        STATIC CHAR16 tempString[2];
        tempString[0] = GetGermanCharacter(Key);
        tempString[1] = L'\0';

        return tempString;
    }
    return (CHAR16*)L"\0";
}