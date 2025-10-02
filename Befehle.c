int row = 0;
void clear(){
    volatile unsigned short* video = (unsigned short*)0xB8000;
    for(int x = 0;x<80; x++){
        for(int y=0;y<25;y++){
            video[y*80+x] = (unsigned short)' ' | (0x07 << 8);
        }
    }
    row=0;
}