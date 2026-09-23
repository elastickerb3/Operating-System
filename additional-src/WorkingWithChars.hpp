#pragma once

#include "./Variables.hpp"
#include "./IntWork.hpp"

CHAR16 to_lower_case(CHAR16 c) {
    if (c >= (CHAR16)L'A' && c <= (CHAR16)L'Z') {
        return c + 32;
    }
    return c;
}

UINT32 LEN(CHAR16* Text){
    UINT32 LENGTH;
    for(LENGTH = 0; Text[LENGTH] != L'\0'; LENGTH++);
    return LENGTH;
}

CHAR16* ConnectText(CHAR16* A, CHAR16* B){
    UINT32 startIndexA = LEN(A); 
    UINT32 lenB = LEN(B);

    for(UINT32 x = 0; x < lenB; x++){
        A[startIndexA + x] = B[x]; 
    }

    A[startIndexA + lenB] = L'\0'; 

    return A; 
}

CHAR16* CharToString(CHAR16 c){
    STATIC CHAR16 string[2];
    string[0] = c;
    string[1] = L'\0';

    return string;
}

BOOLEAN str_cmp(CHAR16* A, CHAR16* B){
    if(LEN(A) != LEN(B)){
        return FALSE;
    }

    for(UINT16 len = 0; len < LEN(A); len++){
        if(A[len] != B[len]){
            return FALSE;
        }
    }

    return TRUE;
}

VOID IntToString(UINT16 val, CHAR16* OutString) {
    UINT8 count = GetDigitCount(val);
    
    OutString[count] = L'\0';
    
    do {
        count--;
        OutString[count] = L'0' + (val % 10);
        val /= 10;
    } while (val > 0);
}