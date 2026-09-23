#pragma once 

#include "./Variables.hpp"

CHAR16 (*parameter(CHAR16* Text, CHAR16* Command))[255] {
    static CHAR16 parameter_Liste[MaxParameterLength][255]; 
	UINTN parameter_index = -1;
	UINTN parameter_index_index = -1;
    
    for (int x = 0; x < MaxParameterLength; x++) {
        for(int y=0;y<255;y++){
			parameter_Liste[x][y] = 0;
		}
    }

    UINTN Ignorirter_Text;
    for(Ignorirter_Text=0;Text[Ignorirter_Text]==' ';Ignorirter_Text++){}

    for(UINTN i=0;Command[i];i++){
        Ignorirter_Text++;
    }
    
	
	for(UINTN i=Ignorirter_Text;Text[i];i++){
		if(Text[i]==' '){
			parameter_index++;
			parameter_index_index=-1;
		}

        for(UINTN a=i;Text[a]==' ';a++){i++;}

		parameter_index_index++;
		parameter_Liste[parameter_index][parameter_index_index] = Text[i];
	}

    return parameter_Liste;
}

BOOLEAN CommandInhalt(CHAR16* text, CHAR16* needle){
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