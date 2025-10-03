int row = 0;

typedef unsigned short uint16_t;
#define PM1a_CNT 0x604
#define SLP_TYP  0x2000
#define SLP_EN   0x2000

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}
void shutdown() {
    outw(PM1a_CNT, SLP_TYP | SLP_EN);
    for(;;) {
       
    }
}

void clear(){
    volatile unsigned short* video = (unsigned short*)0xB8000;
    for(int x = 0;x<80; x++){
        for(int y=0;y<25;y++){
            video[y*80+x] = (unsigned short)' ' | (0x07 << 8);
        }
    }
    row=0;
}