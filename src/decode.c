#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "x86_types.h"
#include "instructions.c"
#include "modrm.c"

const bit8 prefixes[255] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int main() {
    gooblyram *ram = (gooblyram *)calloc(1600000, sizeof(gooblyram)); // 1 byte per index
    /*if (ram == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }*/
    

    bit64 reg[8] = {
        [0] = 0,
        [1] = 0,
        [2] = 0,
        [3] = 0,
        [4] = 0,
        [5] = 0,
        [6] = 0,
        [7] = 0
    };
    //ram[0] = 0x0F; //pf multibyte group
    //ram[1] = 0x2E; //pf CS segment override
   // ram[2] = 0x48; //pf rex.w
    //ram[3] = 0xF5; //op PSADBW

    ram[0] = 0x04;
    ram[1] = 250;

    register bit64 RIP = 0;
    register bit64 multiBytePrefix = 0;
    for (register bit64 cycle = 0; cycle < 1; cycle++) {
        register bit64 RIPDoubleQuadByte = (bit64*)(ram + RIP + 4); //
        
        for (register bit64 index; index < 5; index++) { // TODO: go outside of loop, no reason to check for prefix at 5th byte
            register bit64 offset = index << 3;
            register bit64 byte = ((RIPDoubleQuadByte << offset) & 8);
            if (!(prefixes[byte]))  {
                RIP += index + 1;
                instructions[byte](RIPDoubleQuadByte & offset, ram, RIP, reg);
                break;
            } else if (byte == 0x0F)
                multiBytePrefix = 1;
        }
    }

    printf("%d\n", reg[0]);

    free(ram);
    return 0;
}