typedef unsigned char uint8_t;
typedef unsigned int  uint32_t;
typedef unsigned short uint16_t;

extern void reboot(void);
extern char* key_board_input(void);

#define ROWS 24
#define COLS 80

volatile unsigned short* video = (volatile unsigned short*)0xB8000;
int row = 0;
int printZeichen = 0;
char* Pfad = "/";
int Color = 0x0F;
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
    for (int y=0; y<ROWS; y++) {
        for (int x=0; x<COLS; x++) {
            video[y*COLS+x] = (unsigned short)' ' | (Color << 8);
        }
    }
    row = 0;
    printZeichen = 0;
}

void println(const char* text) {
    int i;
    for(i=0;text[i];i++){
        video[row*COLS+i] = (unsigned short)text[i]|(Color<<8);
    }
    i=i+printZeichen;
    for(;i<COLS;i++){
        video[row*COLS+i]=(unsigned short)' '|(Color<<8);
    }
    if(!(row==ROWS-1)){
        row++;
    }else{
        for(int col = 0; col<COLS;col++){
            video[col]=(unsigned short)0|(Color<<8);
        }
        for(int ROW=1;ROW<ROWS;ROW++){
            for(int col=0;col<COLS;col++){
                video[(ROW-1)*80+col]=video[ROW*80+col];
            }
        }
    }
    printZeichen=0;
}

void print(const char* text) {
    if (!text) return;
    for(int i=printZeichen;i<COLS;i++){
        video[row*80+i] = (unsigned short)' '|(Color<<8);
    }
    for (int i=0; text[i]; i++) {
        video[row*COLS+printZeichen] = (unsigned short)text[i] | (Color<<8);
        printZeichen++;
        if(printZeichen ==COLS){
            row++;
            printZeichen=0;
        }
    }
}

char* input(const char* Text) {
    static char buf[128];
    int pos = 0;

    print(Text);

    while (1) {
        char* c = key_board_input();
        if (c[0] == 1) { 
            break;
        } else if (c[0] == 2) {
            if (pos > 0) {
                pos--;
                buf[pos] = 0;
                if (printZeichen > 0) {
                    printZeichen--;
                    video[row*COLS+printZeichen] = ' ' | (Color<<8);
                }
            }
        } else {
            if (pos < 127) {
                buf[pos++] = c[0];
                buf[pos] = 0;
                video[row*COLS+printZeichen] = (unsigned short)c[0] | (Color<<8);
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
        	println("help      -- diese hife liste");
            println("reboot    -- pc neustart");
            println("shutdown  -- pc herunterfahren");
            println("cls/clear -- console leren");
        }else if(!(cmd[0] == 0)){
            println(combine(cmd, ": befehl nicht gefunden"));
        }
    }
}
