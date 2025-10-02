extern unsigned char get_key(void);
extern void shutdown(void);
typedef unsigned int uint32_t;

#include "Befehle.c"

volatile unsigned short* video = (unsigned short*)0xB8000;
char* Pfad = "/";

int printZeichen = 0;

const char* Zeilen[13] = {
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "",
    "", 
    "",
    ""
};

static unsigned int CommandFind(const char*Main,const char*SuchString){
    unsigned int RETURN = 0;
    for(int i=0;Main[i];i++){
        for(int b=0;SuchString[b];b++){
            if(Main[i+b]==SuchString[b]){
                RETURN++;
            }else{
                RETURN=0;
            }
        }
        if(RETURN){
            break;
        }
    }
    return RETURN;
}

int CharLenght(const char* a){
    int Lenght = 0;
    for(int i=0;a[i];i++){
        Lenght++;
    }
    return Lenght;
}

char* Combine(const char* a, const char* b){
    static char c[9999];
    int c_len = 0;
    for(int i=0;a[i];i++){
        c[c_len] = a[i];
        c_len++;
    }
    for(int i=0;b[i];i++){
        c[c_len] = b[i];
        c_len++;
    }

    return c;
}

static unsigned int strcmp(const char* a, const char* b)
{
    int Test = 0;
    for(int i=0; a[i]&&b[i];i++){
        Test = 1;
        if(a[i] && !b[i]){
            return 0;
        }
        if(b[i] && !a[i]){
            return 0;
        }
        if(b[i] != a[i]){
            return 0;
        }
    }
    return Test;
}

void println(const char* Text){
    int i;
    for(i = 0; Text[i]; i++){
        video[row*80+i] = (unsigned short)Text[i] | (0x07 << 8);
    }

    row++;

    for(i = 0; i<80; i++){
        video[row*80+i] = ' ' | (0x07 << 8);
    }
    printZeichen = 0;

}

void print(const char* Text){
    for (int i = 0; Text[i] != '\0'; i++) {
        video[row*80 + printZeichen] = (unsigned short)Text[i] | (0x07 << 8);
        printZeichen++;
    }
}

char* key_board_input() {
    static char key[2];
    char k;
    
    do {
        k = (char)get_key();
    } while(k == 0); // warten, bis Taste gedrückt
    
    key[0] = k;
    key[1] = '\0';
    
    // hier warten, bis Taste losgelassen wird, um Doppelungen zu vermeiden
    while(get_key() == ' ');

    if(key[0] != 'E' && key[0] != 'S' && key[0] != 'D'){
        video[row*80+printZeichen] = (unsigned short)key[0]|(0x07 << 8);
        printZeichen++;
    }else if(key[0] == 'S'){
        video[row*80+printZeichen] = (unsigned short)' '|(0x07 << 8);
        printZeichen++;
    }else if(key[0] == 'D'){
        video[row*80+printZeichen-1] = (unsigned short)' '|(0x07<<8);
        if(CharLenght(Combine(Pfad, " root# ")) < printZeichen)
        printZeichen--;
    }else{
        row++;
        for(int i=0;i<80;i++){
            video[row*80+i-printZeichen] = ' ' | (0x07<<8);
        }
    }
    

    return key;
}
char* input(const char* Text) {
    static char InputString[9999]; // static to return safely
    int InputInt = 0;

    print(Text);
    
    // Initialize buffer
    for(int i = 0; i < 9999; i++) {
        InputString[i] = '\0';
    }

    while(1) {
        char* InputChar = key_board_input();
        
        if(InputChar[0] == 'E') {  // use single quotes
            break;
        }else if(InputChar[0] == 'D'){
            InputString[InputInt-1] = ' ';
            if(InputInt>0)
            InputInt--;
        }else{
            InputString[InputInt] = InputChar[0];
            InputInt++;
            
            // Safety check to avoid overflow
            if(InputInt >= 9998) {
                break;
            }
        }
    }

    InputString[InputInt] = '\0'; // null-terminate
    printZeichen = 0;
    return InputString;
}

void kernel_main(void) {
    while(1){
        char* Command = input(Combine(Pfad, " root# "));
        if(CommandFind(Command, "clear") || CommandFind(Command, "cls")){
            clear();
        }else if(CommandFind(Command, "shutdown")){
            shutdown();
        }else if(strcmp(Command, "hallo")){
            println("Ebenfalls");
        }
    }
    

    while(1) { __asm__ __volatile__("hlt"); }
}
