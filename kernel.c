#include "kernel_assets/shutdown.c"

typedef unsigned char uint8_t;
typedef unsigned int  uint32_t;
typedef unsigned short uint16_t;

extern char* key_board_input(void);
extern int python(void);
extern int Login(void);
extern void SetNewPasswort(int UserId, char* NewPasswort);

#define ROWS 24
#define COLS 80

#define MaxParameterMenge  25
#define true 1

volatile unsigned short* video = (volatile unsigned short*)0xB8000;
int row = 0;
int printZeichen = 0;
const char* Pfad = "/";

int Color = 0x07;
const char* BackgroundColor="0";
const char* FontColor="7";
char Terminal[(ROWS+1)*COLS] = {0};

#define PM1a_CNT 0x604
#define SLP_TYP  0x2000
#define SLP_EN   0x2000

int getDigitCount(int Start){
    int count = 0;

    while (1){
        if (Start<=0){
            return count;
        }

        Start /= 10;
        count++;
    }
}

char* IntToChar(int Int){
    static char result[100];
    for(int i=0;result[i];i++){
        result[i] = 0;
    }
    int Stelle = 1;
    for(int i=0; i<getDigitCount(Int)-1;i++){
        Stelle *=10;
    }
    for(int i=0; i<getDigitCount(Int);i++){
        int Zahl = (Int / Stelle) % 10;
        Stelle /= 10;

        switch (Zahl){
            case 0:
                result[i] = '0';
                break;
            case 1:
                result[i] = '1';
                break;
            case 2:
                result[i] = '2';
                break;
            case 3:
                result[i] ='3';
                break;
            case 4:
                result[i] = '4';
                break;
            case 5:
                result[i] = '5';
                break;
            case 6:
                result[i] = '6';
                break;
            case 7:
                result[i] = '7';
                break;
            case 8:
                result[i] = '8';
                break;
            case 9:
                result[i] = '9';
                break;
        } 
    }
    return result;
}

void newRow(int i){
    //print kompatiblität
    i=i+printZeichen;
    for(;i<COLS;i++){
		Terminal[row*COLS+i]=' ';
        video[row*COLS+i]=(unsigned short)' '|(Color<<8);
    }
    if(!(row==ROWS-1)){
        row++;
    }else{
        for(int col = 0; col<COLS;col++){
			Terminal[col]=0;
            video[col]=(unsigned short)0|(Color<<8);
        }
        for(int ROW=1;ROW<ROWS;ROW++){
            for(int col=0;col<COLS;col++){
				Terminal[(ROW-1)*80+col]=Terminal[ROW*80+col];
                video[(ROW-1)*80+col]=video[ROW*80+col];
            }
        }
    }
    printZeichen=0;
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

    int t = 0;
    int n = 0;  

    while (text[t] && needle[n]) {
        if (text[t] == ' ') {
            t++;  
            continue;
        }

        if (text[t] != needle[n]) {
            return 0;
        }

        t++;
        n++;
    }

    return needle[n] == '\0' ? 1 : 0;
}

unsigned int CharHex_to_Int(const char* hex) {
    unsigned int result = 0;
    int i = 0;

    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        i = 2;
    }

    for (; hex[i]; i++) {
        result <<= 4;

        if (hex[i] >= '0' && hex[i] <= '9') {
            result += hex[i] - '0';
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            result += hex[i] - 'A' + 10;
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            result += hex[i] - 'a' + 10;
        } else {
            return 7;
        }
    }

    return result;
}

char* combine(const char* a, const char* b) {
    static char buf[128];
    int pos = 0;

    for(int i=0;i<128;i++){
        buf[i]=0;
    }
    if (a) {
        for (int i=0; a[i] && pos < 127; i++) buf[pos++] = a[i];
    }
    if (b) {
        for (int i=0; b[i] && pos < 127; i++) buf[pos++] = b[i];
    }
    return buf;
}

int color(const char* Font, const char* Background){
	if(Font[0]!=0){
		FontColor=Font;
	}else{
		return true;
	}
	if(Background!=0){
		BackgroundColor=Background;
	}
	Color = CharHex_to_Int(combine(BackgroundColor,FontColor));
	for(int x=0;x<COLS*ROWS;x++){
		video[x]=(unsigned short)Terminal[x]|(Color<<8);
	}
	return 0;
}

void println(const char* text) {
    int i;
    //Text Anzeige
    for(i=0;text[i];i++){
		Terminal[row*COLS+i]=text[i];
        video[row*COLS+i] = (unsigned short)text[i]|(Color<<8);
    }

    newRow(i);
}

void print(const char* text) {
    if (!text) return;
    for(int i=printZeichen;i<COLS;i++){
		Terminal[row*80+i]=' ';
        video[row*80+i] = (unsigned short)' '|(Color<<8);
    }
    for (int i=0; text[i]; i++) {
		Terminal[row*80+i]=text[i];
        video[row*COLS+printZeichen] = (unsigned short)text[i] | (Color<<8);
        printZeichen++;
        if(printZeichen ==COLS){
            newRow(printZeichen);
            printZeichen=0;
        }
    }
}

void printInt(int Int){
    print(IntToChar(Int));
}

void printIntln(int Int){
    println(IntToChar(Int));
}

int len(const char* a){
    int i;
    for(i=0;a[i];i++){}
    return i;
}

char* input(const char* Text,int color, int SeeIt) {
    static char buf[128];
    int pos = 0;
    int Color_before=Color;
    Color=color;
    print(Text);
    Color=Color_before;

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
					Terminal[row*COLS+printZeichen]=' ';
                    video[row*COLS+printZeichen] = ' ' | (Color<<8);
                }
            }
        } else if(c[0]==3){
            print("");
        }else{
            if(SeeIt){
                if (pos < 127) {
                    buf[pos++] = c[0];
                    buf[pos] = 0;
				    Terminal[row*COLS+printZeichen]=c[0];
                    video[row*COLS+printZeichen] = (unsigned short)c[0] | (Color<<8);
                    printZeichen++;
                }
            }else{
                if (pos < 127) {
                    buf[pos++] = c[0];
                    buf[pos] = 0;
				    Terminal[row*COLS+printZeichen]=c[0];
                    video[row*COLS+printZeichen] = (unsigned short)'*' | (Color<<8);
                    printZeichen++;
                }
            }
        }
    }
    buf[pos] = 0;
    println("");
    return buf;
}

char (*parameter(const char* Text, const char* Standert_Text))[255] {
    static char parameter_Liste[MaxParameterMenge][255]; 
	int parameter_index = -1;
	int parameter_index_index = -1;
    //An jeder stelle der parameter liste ein null operater einfügen
    for (int x = 0; x < MaxParameterMenge; x++) {
        for(int y=0;y<255;y++){
			parameter_Liste[x][y] = 0;
		}
    }

    //Lehrzeichen Menge for dem eignetlichen parameter herausfinden
    int Ignorirter_Text;
    for(Ignorirter_Text=0;Text[Ignorirter_Text]==' ';Ignorirter_Text++){}

    //Lenge des Standert textes herausfinden und auf Ignorirter Text drauf addiren
    for(int i=0;Standert_Text[i];i++){
        Ignorirter_Text++;
    }
    
	//Mainloop
	for(int i=Ignorirter_Text;Text[i];i++){
		//Die weiteren lehrzeichen entfernen und die parameter hinzufüg größe um 1 erhöen
		if(Text[i]==' '){
			parameter_index++;
			parameter_index_index=-1;
		}

        for(int a=i;Text[a]==' ';a++){i++;}

		//parameter zur liste hinzufügen
		parameter_index_index++;
		parameter_Liste[parameter_index][parameter_index_index] = Text[i];
	}

    //Liste returnen
    return parameter_Liste;
}

void PrintMiddle(const char* Text){
    int Formel=(COLS/2)-(len(Text)/1.5);
    printZeichen = Formel;
    print(Text);
    printZeichen=0;
    row++;
}

void ultimate_clear(){
    for (int y=0; y<ROWS+1; y++) {
        for (int x=0; x<COLS; x++) {
			Terminal[y*COLS+x] = ' ';
            video[y*COLS+x] = (unsigned short)' ' | (Color << 8);
        }
    }
    row = 0;
    printZeichen = 0;
} 

void clear() {
    ultimate_clear();
    PrintMiddle("Potato Os");
}

void kernel_main(void) {
    clear();
    Login();
    ultimate_clear();

    PrintMiddle("Willkommen zu Potato Os!");
    println("help fuer hilfe.");
    while (1) {
        char* cmd = input(combine(Pfad," root# "),Color, 1);
        int LehrzeichenTest=1;
        for(int i=0;cmd[i];i++){
            if(cmd[i]!=' '){
                LehrzeichenTest=0;
            }
        }
        if(LehrzeichenTest) continue;

        if(cmd[0]==0){}else if (CommandInhalt(cmd, "clear") || CommandInhalt(cmd, "cls")) {
            clear();
        } else if (CommandInhalt(cmd, "reboot")) {
            break;
        } else if (CommandInhalt(cmd, "shutdown")) {
            shutdown();
        } else if (CommandInhalt(cmd, "help")){
        	println("help        -- diese hilfe liste");
            println("reboot      -- pc neustart");
            println("shutdown    -- pc herunterfahren");
            println("cls/clear   -- console leeren");
            println("color       -- Farbe von Hintergrund und schrift aendern");
            println("ascii-table -- alle zahlen mit dem passendem asicii ergebnis");
        }else if(CommandInhalt(cmd, "color")){
            if(str_cmp(parameter(cmd,"color")[0],"/?")||str_cmp(parameter(cmd, "color")[0],"--help")){
                println("color [Schriftfarbe in hex] [optional Hintergrund farbe]");
                println("Fraben:");
                println("1:dunkelblau, 2:dunkelgruen");
                println("3:tyrkis, 4:rot, 5:pink");
                println("6:orange, 7:hellgrau");
                println("8:dunkelgrau,9:violett");
                println("a:hellgruen, b:hellblau");
                println("c:hellrot,d:lila");
                println("e:gelb, f:weis");
            }else if(str_cmp(parameter(cmd,"color")[0],parameter(cmd,"color")[1])){
                println("Die Schrift farbe darf nicht gleich mit der Hintergrund farbe sein!");
            }else if(CharHex_to_Int(parameter(cmd,"color")[0])+CharHex_to_Int(BackgroundColor)==0){
                println("Die Schrift farbe darf nicht Null sein!");
            }else{
                if(color(parameter(cmd,"color")[0], parameter(cmd,"color")[1])){
                    println("Ungülige parameter!");
                }
            }
        }else if(CommandInhalt(cmd,"ascii-table")){
            for(int i=1;i<128;i++){
                char Char[2];
                Char[0]=(char)i;
                Char[1]=0;
                printInt(i);
                print(":");
                if(i!=127){
                    print(combine(Char, ","));
                }else{
                    print(Char);
                }
            }
            println("");
        }else if(CommandInhalt(cmd,"python")){
            python();
        }else  {
            println(combine(cmd,": Befehl nicht gefunden."));
        }
    }
}
