#pragma once

#include "./Variables.hpp"

UINT8 GetDigitCount(UINT16 val) {
    UINT8 count = 0;
    do {
        count++;
        val /= 10;
    } while (val > 0);
    return count;
}

UINT32 paw(UINTN Basis, UINTN Potenz){
    UINT32 INT = 1;

    for(UINTN i=0;i<Potenz;i++){
        INT *= Basis;
    }

    return INT;
}