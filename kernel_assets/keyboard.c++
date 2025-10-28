#include "scancodes.hpp"

extern "C" {
    unsigned int get_scancode();
    int str_cmp(const char* a, const char* b);
}

char KeyLanguage[3] = "de";
int shift_pressed = 0;
int caps_lock = 0;

char translate_scancode(unsigned char scancode) {
    char c;
    
    if (scancode & 0x80) {
        unsigned char key = scancode & 0x7F;
        if (key == 0x2A || key == 0x36) shift_pressed = 0;
        return 0;
    }

    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return 0;
    }

    if (scancode == 0x3A) {
        caps_lock = !caps_lock;
        return 0;
    }
    if(scancode==0x48){
        return 3;
    }
    if(scancode==0x50){
        return 4;
    }

    c = shift_pressed ? de_scancode_shift_map[scancode] : de_scancode_map[scancode];
    if (c >= 'a' && c <= 'z') {
        if (caps_lock ^ shift_pressed) {
            c = c - ('a' - 'A');
        }
    }
    return c;
}

extern "C" char* key_board_input(){
    static char key[2];
    key[0]=translate_scancode(get_scancode()); 
    if(key[0]==0)key_board_input();
    key[1]=0;
    return key;
}