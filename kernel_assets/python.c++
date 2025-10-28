extern "C" {
    char* input(const char* Text,int color);
    void println(const char* Text);//Zum Debug
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
            char* Input = input(">",9);
            if(str_cmp(Input,"RUN")){
                break;
            }else if(str_cmp(Input,"exit")){
                return 0;
            } 
            for(int y=0;Input[y];y++){
                Code[i][y]=Input[y];
            }
        } 
        println(Code[0]);
    }
}