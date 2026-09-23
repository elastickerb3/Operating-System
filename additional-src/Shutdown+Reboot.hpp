#pragma once

#include "./Variables.hpp"

void shutdown(){
    SystemTable->RuntimeServices->ResetSystem(
        EfiResetShutdown,
        EFI_SUCCESS,
        0,
        NULL
    );
}

void reboot(){
    SystemTable->RuntimeServices->ResetSystem(
        EfiResetCold,
        EFI_SUCCESS,
        0,
        NULL
    );
}