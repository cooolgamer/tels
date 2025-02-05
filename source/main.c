// DSES - Dead Simple EULA Set
// Copyright (C) 2018 Valentijn "noirscape" V.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>

int main(int argc, char* argv[])
{
    // noop call to shut up the compiler
    (void)argc;
    (void)argv;

    // Service initialization
    aptInit();
    gfxInitDefault();
    hidInit();
    cfguInit();
    // Variables
    Result res;
    bool done = false;
    uint8_t langData[1];
    u8 regionCode;

    PrintConsole topScreen;
    consoleInit(GFX_TOP, &topScreen);
    consoleSelect(&topScreen);

    printf("Taiwan English Language Setter - v1.1.0\n\n");

    CFGU_SecureInfoGetRegion(&regionCode); // check if it's taiwan
    if (regionCode != 6){
        printf("This program is for Taiwan systems only.\n\nPress Start to exit.");
        done = true;
    }
    
    // read magic
    res = CFGU_GetConfigInfoBlk2(1, 0xA0002, langData);
    if (R_FAILED(res))
    {
        printf("Something weird happened.\nCouldn't get language data.\n\nPress Start to exit.");
        done = true;
    }

    if(!done)
        printf(langData[0] == 0x01 ? "Press A to set the system language to Chinese.\n\n" : "Press A to set the system language to English.\n\n");

    while(aptMainLoop())
    {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_A && !done)
        {
            if (langData[0] == 0x01) // 01 == English have been set, so set it to Chinese.
                langData[0] = 0x0B;
            else // Anything else: set the language to English.
                langData[0] = 0x01;
            res = CFG_SetConfigInfoBlk8(1, 0xA0002, langData);
            if(R_FAILED(res))
                printf("Something went wrong...\n\n");
            else{
                printf(langData[0] == 0x01 ? "Setting language to English succeeded.\n\nWARNING: Launching System Settings will set\nthe language back to Chinese!\n\n" : "Setting language to Chinese succeeded.\n\n");
                printf("NOTE: Launch the friend list or game notes\nthen exit to apply changes!\n\nPress START to exit.\n\n");
                done = true;
            }
        }
        if (kDown & KEY_START)
        {
            printf("Exiting...\n");
            break;
        }
    }

    aptExit();
    gfxExit();
    hidExit();
    cfguExit();
    return 0;
}
