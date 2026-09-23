#pragma once

#include "./WorkingWithChars.hpp"

UINT32 BIOS_IndexIntoHex(CHAR16* c){
    CHAR16 c_lower = to_lower_case(c[0]);
    if(c_lower == (CHAR16)L'0'){
        return 0x000000;
    }else if(c_lower == (CHAR16)L'1'){
        return 0x0000AA;
    }else if(c_lower == (CHAR16)L'2'){
        return 0x00AA00;
    }else if(c_lower == (CHAR16)L'3'){
        return 0x00AAAA;
    } else if(c_lower == (CHAR16)L'4'){
        return 0xAA0000;
    } else if(c_lower == (CHAR16)L'5'){
        return 0xAA00AA;
    } else if(c_lower == (CHAR16)L'6'){
        return 0xAA5500;
    }else if(c_lower == (CHAR16)L'7'){
        return 0xAAAAAA;
    }else if(c_lower == (CHAR16)L'8'){
        return 0x555555;
    }else if(c_lower == (CHAR16)L'9'){
        return 0x5555FF;
    }else if(c_lower == (CHAR16)L'a'){
        return 0x55FF55;
    }else if(c_lower == (CHAR16)L'b'){
        return 0x55FFFF;
    }else if(c_lower == (CHAR16)L'c'){
        return 0xFF0000;
    }else if(c_lower == (CHAR16)L'd'){
        return 0xFF00FF;
    }else if(c_lower == (CHAR16)L'e'){
        return 0xFFFF55;
    }else if(c_lower == (CHAR16)L'f'){
        return 0xFFFFFF;
    }
    return 0x000000;
}