extern "C" {
    void println(const char* Text);
    char* input(const char* Text, int Color, int SeeIt);
    int str_cmp(const char* a, const char* b);
}

extern "C" int python(){
    while (1){
        char Code[999][128];
        for(int x=0;x<999;x++){
            for(int y=0;y<128;y++){
                Code[x][y]=0;        
            }
        }
        for(int i=0;i<999;i++){
            char* Input = input(">",9, 1);
            if(str_cmp(Input,"RUN")){
                break;
            }else if(str_cmp(Input,"exit")){
                return 0;
            } 
            for(int y=0;Input[y];y++){
                Code[i][y]=Input[y];
            }
        } 
        
    }
}