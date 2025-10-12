typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

extern "C" {
    u32 get_scancode();
}

int shift_pressed = 0;
int caps_lock = 0;

char scancode_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', 0, 0, 2, 0,
    'q', 'w', 'e', 'r', 't', 'z', 'u', 'i',
    'o', 'p', 0, '+', 1, 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', 0,
    0, '^', 0, '#', 'y', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '-', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0
};

char scancode_shift_map[128] = {
    0, 27, '!', '"', 0, '$', '%', '&',
    '/', '(', ')', '=', '?', 0, 0, '\t',
    'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I',
    'O', 'P', 0, '*', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', 0,
    0, 0, 0, '\'', 'Y', 'X', 'C', 'V',
    'B', 'N', 'M', ';', ':', '_', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0
};

char translate_scancode(u8 scancode) {
    if (scancode & 0x80) {
        u8 key = scancode & 0x7F;
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

    char c = shift_pressed ? scancode_shift_map[scancode] : scancode_map[scancode];
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