#define true 1
#define false 0

extern "C"{
    char* input(const char* Text,int color);
    void println(const char* Text);
    int str_cmp(const char* a, const char* b);
    void clear();
    int __cxa_guard_acquire(long long* g) {
       return !*g;
    }

    void __cxa_guard_release(long long* g) {
        *g = 1;
    }

    void __cxa_guard_abort(long long*) {}
} 

char* UsernamePasswort[2] ={"123", ""}; 

extern "C" int Login(){
    int UserID;
    while(true){
        int LogInSursfully=false;
        static char* Usernames[3][2] ={{"root",UsernamePasswort[0]} ,{ "gast", UsernamePasswort[1]}};
        
        char* Username= input("Zum Login Username Eingeben: ", 0x07);
        for(int i=0;Usernames[i][0];i++){
            if(!str_cmp(Username,Usernames[i][0])){
                continue;
            } 
            char* Passwort=input("Passwort eingeben: ",0x07);
            if(str_cmp(Passwort,Usernames[i][1])){
                println("Login war erfolgreich!");
                println(UsernamePasswort[i]);
                LogInSursfully=true;
                UserID=i;
                break;
            } 
        } 
        if(LogInSursfully){
            break;
        } 
    } 
    clear();
    return UserID;
} 