#define true 1
#define false 0

extern "C"{
    char* input(const char* Text,int color, int SeeIt);
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

char* UsernamePasswort[2] ={"1234", ""}; 

char* names[2] = {"root", "gast"};

char* get_Passwort(int UserID){
    return UsernamePasswort[UserID];
}

char* get_Username(int UserID){
    return names[UserID];
}

extern "C" int Login(){
    int UserID;
    while(true){
        int LogInSursfully=false;
        static char* Usernames[3][2] ={{get_Username(0), get_Passwort(0)} ,{get_Username(1), get_Passwort(1)}};
        
        char* Username= input("Zum Login Username Eingeben: ", 0x07, 1);
        for(int i=0;Usernames[i][0];i++){
            if(!str_cmp(Username,Usernames[i][0])){
                continue;
            } 
            char* Passwort=input("Passwort eingeben: ",0x07, 0);
            if(str_cmp(Passwort,Usernames[i][1])){
                println("Login war erfolgreich!");
                println(get_Passwort(i));
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