extern unsigned char get_key(void);
extern void reboot(void);

typedef unsigned int  uint32_t;
typedef unsigned short uint16_t;

#define ROWS 13
#define COLS 80

volatile unsigned short* video = (volatile unsigned short*)0xB8000;
int row = 0;
int printZeichen = 0;
char* Pfad = "/";

const char* Zeilen[ROWS] = { "", "", "", "", "", "", "", "", "", "", "", "", "" };
#define PM1a_CNT 0x604
#define SLP_TYP  0x2000
#define SLP_EN   0x2000

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

void shutdown() {
    outw(PM1a_CNT, SLP_TYP | SLP_EN);
    for(;;) { __asm__ volatile("hlt"); }
}
int len(const char* s) {
    int len = 0;
    if (!s) return 0;
    while (s[len]) len++;
    return len;
}

int str_cmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return (a[i] == 0 && b[i] == 0);
}

int CommandInhalt(const char* text, const char* needle) {
    if (!text || !needle) return 0;
    for (int i = 0; text[i]; i++) {
        int j = 0;
        while (needle[j] && text[i+j] && text[i+j] == needle[j]) j++;
        if (needle[j] == 0) return 1;
    }
    return 0;
}

char* combine(const char* a, const char* b) {
    static char buf[128];
    int pos = 0;

    if (a) {
        for (int i=0; a[i] && pos < 127; i++) buf[pos++] = a[i];
    }
    if (b) {
        for (int i=0; b[i] && pos < 127; i++) buf[pos++] = b[i];
    }
    buf[pos] = 0;
    return buf;
}
void clear() {
    for (int y=0; y<25; y++) {
        for (int x=0; x<COLS; x++) {
            video[y*COLS+x] = (unsigned short)' ' | (0x07 << 8);
        }
    }
    for (int i=0; i<ROWS; i++) Zeilen[i] = "";
    row = 0;
    printZeichen = 0;
}

void render_Text() {
    for (int y=0; y<ROWS; y++) {
        int len = len(Zeilen[y]);
        for (int x=0; x<COLS; x++) {
            char c = (x < len) ? Zeilen[y][x] : ' ';
            video[y*COLS+x] = (unsigned short)c | (0x07 << 8);
        }
    }
}

void println(const char* text) {
    if (row >= ROWS) {
        for (int i=1; i<ROWS; i++) Zeilen[i-1] = Zeilen[i];
        Zeilen[ROWS-1] = text;
    } else {
        Zeilen[row] = text;
        row++;
    }
    render_Text();
    printZeichen = 0;
}

void print(const char* text) {
    if (!text) return;
    for (int i=0; text[i]; i++) {
        video[row*COLS+printZeichen] = (unsigned short)text[i] | (0x07<<8);
        printZeichen++;
    }
}

char* key_board_input() {
    static char key[2];
    unsigned char k;

    do { k = get_key(); } while (k == 0);

    key[0] = (char)k;
    key[1] = 0;

    while (get_key() != 0);

    return key;
}

char* input(const char* Text) {
    static char buf[128];
    int pos = 0;

    print(Text);

    while (1) {
        char* c = key_board_input();
        if (c[0] == 'E') { 
            break;
        } else if (c[0] == 'D') {
            if (pos > 0) {
                pos--;
                buf[pos] = 0;
                if (printZeichen > 0) {
                    printZeichen--;
                    video[row*COLS+printZeichen] = ' ' | (0x07<<8);
                }
            }
        } else {
            if (pos < 127) {
                buf[pos++] = c[0];
                buf[pos] = 0;
                video[row*COLS+printZeichen] = (unsigned short)c[0] | (0x07<<8);
                printZeichen++;
            }
        }
    }

    buf[pos] = 0;
    println("");
    return buf;
}

void kernel_main(void) {
    println("Willkommen!");

    while (1) {
        char* cmd = input(combine(Pfad, " root# "));

        if (CommandInhalt(cmd, "clear") || CommandInhalt(cmd, "cls")) {
            clear();
        } else if (CommandInhalt(cmd, "reboot")) {
            reboot();
        } else if (CommandInhalt(cmd, "shutdown")) {
            shutdown();
        } else if (CommandInhalt(cmd, "help")){
        	println("\n");
        }
    }
}
