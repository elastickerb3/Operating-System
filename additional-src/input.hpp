#pragma once

#include "./Variables.hpp"
#include "./keyboardinput.hpp"
#include "./terminal.hpp"

CHAR16* input(const wchar_t* Text, BOOLEAN See){
    STATIC CHAR16 Output[InputMaxLength];
    INT16 OutputIndex = -1;
    print((wchar_t*)Text, FALSE);

    Output[0] = (CHAR16)L'\0';

    while (TRUE) {
        CHAR16* c = key_board_input(SystemTable);

        if (c[0] == L'\0')
            continue;

        if (c[0] == L'\n' || c[0] == L'\r') {
            print(L"\n", FALSE);
            Output[OutputIndex + 1] = L'\0';
            break;
        }

        if (c[0] == L'\b') {
            if (OutputIndex >= 0) {
                OutputIndex--;
                Output[OutputIndex + 1] = L'\0';

                print(L"\b", TRUE);
            }

            continue;
        }

        if (See) {
            if (OutputIndex < InputMaxLength - 1) {
                OutputIndex++;
                Output[OutputIndex] = c[0];
                Output[OutputIndex + 1] = L'\0';

                print((wchar_t*)c, TRUE);
            }
        } else {
            if (OutputIndex < InputMaxLength - 1) {
                OutputIndex++;
                Output[OutputIndex] = c[0];
                Output[OutputIndex + 1] = L'\0';

                print(L"*", TRUE);
            }
        }
    }

    return Output;
}
