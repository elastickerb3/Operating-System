#pragma once

#include <efi.h>

#include "UsingQemu.hpp"

#define TERMINAL_MAX_CHARS 4096
#define InputMaxLength 1024
#define MaxPathLength 4096
#define MaxIntString 1024
#define MaxParameterLength 1024

#define SpecialCodesLength 7

#define RESERVED_BOTTOM_LINES 2

#define COLOR_WHITE 0xffffff
#define COLOR_BLACK 0x000000

typedef struct{
    BOOLEAN Removable;
    CHAR16 CHAR;
} Terminal;

Terminal terminal[TERMINAL_MAX_CHARS];

UINTN terminal_length = 0;
UINTN terminal_columns = 0;
UINTN terminal_rows = 0;

BOOLEAN WriteIntoTerminalBuffer = TRUE;

EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop = nullptr;

EFI_SYSTEM_TABLE *SystemTable;

typedef struct {
    UINT16 SearchScanCode;
    CHAR16 SearchUnicode;
    CHAR16 RealGermanChar;
} DE_KEY_MAP;

const DE_KEY_MAP ScanCodeLookUpTableDE[162] = {
    {0x0000, L'1',  L'1'}, {0x0000, L'!',  L'!'},
    {0x0000, L'2',  L'2'}, {0x0000, L'@',  L'"'}, 
    {0x0000, L'3',  L'3'}, {0x0000, L'#',  L'§'}, 
    {0x0000, L'4',  L'4'}, {0x0000, L'$',  L'$'},
    {0x0000, L'5',  L'5'}, {0x0000, L'%',  L'%'},
    {0x0000, L'6',  L'6'}, {0x0000, L'^',  L'&'}, 
    {0x0000, L'7',  L'7'}, {0x0000, L'&',  L'/'}, 
    {0x0000, L'8',  L'8'}, {0x0000, L'*',  L'('}, 
    {0x0000, L'9',  L'9'}, {0x0000, L'(',  L')'}, 
    {0x0000, L'0',  L'0'}, {0x0000, L')',  L'='}, 
    
    {0x0000, L'-',  L'ß'}, {0x0000, L'_',  L'?'},
    {0x0000, L'=',  L'´'}, {0x0000, L'+',  L'`'},
    
    {0x0000, L'z',  L'y'}, {0x0000, L'Z',  L'Y'},
    {0x0000, L'y',  L'z'}, {0x0000, L'Y',  L'Z'},

    {0x0000, L';',  L'ö'}, {0x0000, L':',  L'Ö'},
    {0x0000, L'\'', L'ä'}, {0x0000, L'"',  L'Ä'},
    {0x0000, L'[',  L'ü'}, {0x0000, L'{',  L'Ü'}, 
    {0x0000, L']',  L'+'}, {0x0000, L'}',  L'*'}, 
    {0x0000, L'\\', L'#'}, {0x0000, L'|',  L'\''},
    {0x0000, L'/',  L'-'}, {0x0000, L'?',  L'_'}, 
    {0x0000, L'`',  L'^'}, {0x0000, L'~',  L'°'}, 
    {0x0000, L'<',  L';'}, {0x0000, L'>',  L':'}, 

    {0x0001, 0x0000, L'\0'},
    {0x0002, 0x0000, L'\0'}, 
    {0x0003, 0x0000, L'\0'}, 
    {0x0004, 0x0000, L'\0'}, 
    {0x0005, 0x0000, L'\0'},
    {0x0006, 0x0000, L'\0'},
    {0x0007, 0x0000, L'\0'}, 
    {0x0008, 0x0000, L'\0'}, 
    {0x0009, 0x0000, L'\0'},
    {0x000A, 0x0000, L'\0'},
    {0x0017, 0x0000, L'\0'},
    
    {0x000B, 0x0000, L'\0'}, {0x000C, 0x0000, L'\0'}, {0x000D, 0x0000, L'\0'},
    {0x000E, 0x0000, L'\0'}, {0x000F, 0x0000, L'\0'}, {0x0010, 0x0000, L'\0'},
    {0x0011, 0x0000, L'\0'}, {0x0012, 0x0000, L'\0'}, {0x0013, 0x0000, L'\0'},
    {0x0014, 0x0000, L'\0'}, {0x0015, 0x0000, L'\0'}, {0x0016, 0x0000, L'\0'},

    {0x0000, L'a', L'a'}, {0x0000, L'b', L'b'}, {0x0000, L'c', L'c'}, {0x0000, L'd', L'd'},
    {0x0000, L'e', L'e'}, {0x0000, L'f', L'f'}, {0x0000, L'g', L'g'}, {0x0000, L'h', L'h'},
    {0x0000, L'i', L'i'}, {0x0000, L'j', L'j'}, {0x0000, L'k', L'k'}, {0x0000, L'l', L'l'},
    {0x0000, L'm', L'm'}, {0x0000, L'n', L'n'}, {0x0000, L'o', L'o'}, {0x0000, L'p', L'p'},
    {0x0000, L'q', L'q'}, {0x0000, L'r', L'r'}, {0x0000, L's', L's'}, {0x0000, L't', L't'},
    {0x0000, L'u', L'u'}, {0x0000, L'v', L'v'}, {0x0000, L'w', L'w'}, {0x0000, L'x', L'x'},
    {0x0000, L'A', L'A'}, {0x0000, L'B', L'B'}, {0x0000, L'C', L'C'}, {0x0000, L'D', L'D'},
    {0x0000, L'E', L'E'}, {0x0000, L'F', L'F'}, {0x0000, L'G', L'G'}, {0x0000, L'H', L'H'},
    {0x0000, L'I', L'I'}, {0x0000, L'J', L'J'}, {0x0000, L'K', L'K'}, {0x0000, L'L', L'L'},
    {0x0000, L'M', L'M'}, {0x0000, L'N', L'N'}, {0x0000, L'O', L'O'}, {0x0000, L'P', L'P'},
    {0x0000, L'Q', L'Q'}, {0x0000, L'R', L'R'}, {0x0000, L'S', L'S'}, {0x0000, L'T', L'T'},
    {0x0000, L'U', L'U'}, {0x0000, L'V', L'V'}, {0x0000, L'W', L'W'}, {0x0000, L'X', L'X'},
    
    {0x0000, L'\n', L'\n'}, {0x0000, L'\n', L'\n'}, {0x0000, L'\b', L'\b'}, {0x0000, L'\t', L'\t'},
    {0x0000, L' ', L' '},

};

UINT32 screen_width  = Gop->Mode->Info->HorizontalResolution;
UINT32 screen_height = Gop->Mode->Info->VerticalResolution;

UINT8 char_width = 32;
UINT8 char_heigth = 32;

UINT8 line_spacing = 8;
UINT8 char_spacing = 2;

UINT32 x = 0;
UINT32 y = 0;

CHAR16 PATH[MaxPathLength];

EFI_BOOT_SERVICES* gBS = nullptr;

UINTN BG_COLOR = COLOR_BLACK;
UINTN FONT_COLOR = COLOR_WHITE;

UINT32* ShadowFrameBuffer = nullptr;

void ResetVariables(){
    terminal_columns =
        Gop->Mode->Info->HorizontalResolution /
        (char_width + char_spacing);

    terminal_rows =
        Gop->Mode->Info->VerticalResolution /
        (char_heigth + line_spacing);

    PATH[0] = (CHAR16)L'\0';
    PATH[0] = (CHAR16)L'\\';
}
