#pragma once

#include <efi.h>
#include "./Variables.hpp"
#include "./font.hpp"

BOOLEAN CheckIfTheTerminalIsFull(){
    UINTN UsedRows = (terminal_length + terminal_columns - 1) / terminal_columns; 

    for(UINTN i = 0; terminal[i].CHAR != L'\0'; i++){
        if(terminal[i].CHAR == L'\n' || terminal[i].CHAR == L'\r'){
            UsedRows++;
        }
    }

    if (UsedRows + 2 >= terminal_rows) {
        return TRUE;
    }
    return FALSE;
}

void drawPixel(UINTN x, UINTN y, UINT32 color) {
    if (Gop == nullptr) return;
    
    UINT32 width = Gop->Mode->Info->HorizontalResolution;
    UINT32 height = Gop->Mode->Info->VerticalResolution;
    
    if (x >= width || y >= height) return;

    if (ShadowFrameBuffer != nullptr) {
        ShadowFrameBuffer[y * width + x] = color;
    }

    UINT32* framebuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINT32* pixel_address = framebuffer + (y * Gop->Mode->Info->PixelsPerScanLine) + x;
    *pixel_address = color;
}

void clear_first_line(UINT32 color) {
    if (terminal_length == 0) return;

    UINTN total_width_pixels = terminal_columns * 32;
    for (UINTN screen_x = 0; screen_x < total_width_pixels; screen_x++) {
        for (UINTN tempY = 0; tempY < 32; tempY++) {
            drawPixel(screen_x, tempY, color);
        }
    }

    for (UINTN i = 0; i < terminal_columns; i++) {
        if (i < terminal_length) { 
            terminal[i].CHAR = (CHAR16)L'\0';
            terminal[i].Removable = TRUE;
        }
    }

    if (y == 0) {
        x = 0;
    }
}

void clear(UINT32 color) {
    if (Gop == nullptr) return;

    UINT32 width = Gop->Mode->Info->HorizontalResolution;
    UINT32 height = Gop->Mode->Info->VerticalResolution;
    UINT32 scanline = Gop->Mode->Info->PixelsPerScanLine;
    UINT32* framebuffer = (UINT32*)Gop->Mode->FrameBufferBase;

    for (UINT32 y = 0; y < height; y++) {
        UINT32* row = framebuffer + (y * scanline);
        for (UINT32 x = 0; x < width; x++) {
            row[x] = color;
        }
    }

    if (ShadowFrameBuffer != nullptr) {
        UINTN total_pixels = (UINTN)width * height;
        for (UINTN i = 0; i < total_pixels; i++) {
            ShadowFrameBuffer[i] = color;
        }
    }

    if(WriteIntoTerminalBuffer){
        for(UINT16 i = 0; i<TERMINAL_MAX_CHARS;i++){
            terminal[i].CHAR = (CHAR16)L' ';
        }
        terminal_length = 0;
    }
    
    x=0;
    y=0;
}

void terminal_add_char(CHAR16 c, BOOLEAN Removable) {
    if (terminal_length >= TERMINAL_MAX_CHARS - 1) {
        UINTN drop = TERMINAL_MAX_CHARS / 2;
        for (UINTN i = drop; i < terminal_length; i++)
            terminal[i - drop] = terminal[i];
        terminal_length -= drop;
    }

    terminal[terminal_length].CHAR = c;
    terminal[terminal_length].Removable = Removable;

    terminal_length++;

    terminal[terminal_length].CHAR = L'\0';
    terminal[terminal_length].Removable = FALSE;
}

void scroll_terminal_buffer() {
    if (terminal_length == 0) return;

    UINTN remove = 0;
    while (remove < terminal_length && remove < terminal_columns) {
        CHAR16 c = terminal[remove].CHAR;
        remove++;
        if (c == L'\n' || c == L'\r') {
            break;
        }
    }

    if (remove == 0 || remove > terminal_length) return;

    for (UINTN i = remove; i <= terminal_length; i++) {
        terminal[i - remove] = terminal[i];
    }

    terminal_length -= remove;
}

void scroll_up() {
    if (Gop == nullptr || gBS == nullptr || ShadowFrameBuffer == nullptr) return;

    UINT32 width    = Gop->Mode->Info->HorizontalResolution;
    UINT32 height   = Gop->Mode->Info->VerticalResolution;
    UINT32 step     = char_heigth + line_spacing; 

    if (step >= height) return;

    UINTN bytes_to_copy = (UINTN)(height - step) * width * sizeof(UINT32);

    typedef VOID (EFIAPI *EFI_NATIVE_COPY_MEM)(VOID *Destination, VOID *Source, UINTN Length);
    ((EFI_NATIVE_COPY_MEM)(gBS->CopyMem))(ShadowFrameBuffer, ShadowFrameBuffer + ((UINTN)step * width), bytes_to_copy);

    UINT32* dst_bottom = ShadowFrameBuffer + (UINTN)(height - step) * width;
    UINTN pixels_to_clear = (UINTN)step * width;
    for (UINTN i = 0; i < pixels_to_clear; i++) {
        dst_bottom[i] = BG_COLOR;
    }

    Gop->Blt(Gop, 
             reinterpret_cast<EFI_GRAPHICS_OUTPUT_BLT_PIXEL*>(ShadowFrameBuffer), 
             EfiBltBufferToVideo, 
             0, 0, 0, 0, width, height, width * sizeof(UINT32));

    if (y >= step) y -= step; else y = 0;

    if (WriteIntoTerminalBuffer) {
        scroll_terminal_buffer();
    }
}

void newline(){
    x = 0;
    y += char_heigth + line_spacing;

    if (Gop != nullptr) {
        UINT32 reserved = RESERVED_BOTTOM_LINES * (char_heigth + line_spacing);
        UINT32 height   = Gop->Mode->Info->VerticalResolution;
        UINT32 usable   = (height > reserved) ? (height - reserved) : height;

        if (y + char_heigth > usable) {
            scroll_up();
        }
    }
}

void clear_last_char(UINT32 color) {
    if (terminal_length == 0) return;
    terminal_length--; 

    terminal[terminal_length].CHAR = (CHAR16)L'\0';
    terminal[terminal_length].Removable = TRUE;

    if (x >= (32U + char_spacing)) {
        x -= (32 + char_spacing);
    } else {
        x = 0; 
    }

    for (UINTN tempX = 0; tempX < 32; tempX++) {
        for (UINTN tempY = 0; tempY < 32; tempY++) {
            drawPixel(x + tempX, y + tempY, color);
        }
    }
}

UINT32 GetCharIdx(wchar_t c){
    for(UINT8 i = 0; i < SpecialCodesLength; i++){
        if(c == SpecialCodes[i]){
            return i + 127;
        }
    }

    return (UINT8)c;
}

void draw_font(UINTN start_x, UINTN start_y, CHAR16 c, UINT32 text_color, UINT32 BG_COLOR, BOOLEAN Removable) {
    UINT32 screen_width = Gop->Mode->Info->HorizontalResolution;
    
    if (c == L'\r' || c == L'\n') {
        newline();
        if(WriteIntoTerminalBuffer) terminal_add_char(c, Removable);
        return;
    }

    if (x + char_width+char_spacing > screen_width) {
        newline();
        start_x = x;
        start_y = y;
    }

    if (c == L'\b') {
        if (terminal_length > 0) {
            UINTN index = terminal_length - 1;

            if (!terminal[index].Removable) return;

            clear_last_char(BG_COLOR);
        }
        return;
    }
    
    
    const UINT32* char_array = font[GetCharIdx(c)];

    for (UINTN y = 0; y < 32; y++) {
        UINT32 row_data = char_array[y];
        
        for (UINTN x = 0; x < 32; x++) {
            UINT32 bit_mask = (1UL << (31 - x));
            
            if (row_data & bit_mask) {
                drawPixel(start_x + x, start_y + y, text_color);
            } else {
                drawPixel(start_x + x, start_y + y, BG_COLOR);
            }
        }
    }

    if (c != L'\n' && c != L'\r') {
        x += char_width+char_spacing;
    }

    if(WriteIntoTerminalBuffer){
        terminal_add_char(c, Removable);
    }
}

void print(const wchar_t* Text, BOOLEAN Removable) {
    if (Gop == nullptr) return;

    for (UINT32 i = 0; Text[i] != L'\0'; i++) {
        wchar_t c = Text[i];

        draw_font(x, y, c, FONT_COLOR, BG_COLOR, Removable);
    }
}