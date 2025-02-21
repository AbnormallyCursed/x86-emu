#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "x86_types.h"
//#include "instructions.h"

const bit32 prefixCombination[810000] = {}; // TODO: goobly combination prefix
const bit8 prefixes[244] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1};

int static add(register bit32 prefix, gooblyram *ram, register bit32 EIP, bit64 reg[8]) {
    if (prefix != 0) {        
        if ((prefix & 8 == 0x48) || ((prefix >> 8) & 8 == 0x48) || ((prefix >> 16) & 8 == 0x48) || ((prefix >> 24) & 8 == 0x48)) { // TODO: alternative benchmark test memory vs instruction
            // REX.W
            
        }
    } else {
        printf("meoowwwwwwwwwwwww %d\n", ram[EIP]);
        reg[0] += ram[EIP];
        return EIP++;
    }
}

int static (*instructions[])(bit32, gooblyram*, bit32, bit64[8]) = {add};

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

    register bit32 EIP = 0;
    //while (1) {
        register bit8 multiByteMeoww = 0;
        register bit32 EIPQuadByte = (bit32*)(ram + EIP + 4); //
        register bit8 byte = (bit8)(EIPQuadByte & 8);

        if ((byte >= 0x0F) && prefixes[byte]) {
            if (byte == 0x0F)
                multiByteMeoww = 1;
            byte = (EIPQuadByte >> 8) & 8;
            if ((byte >= 0x0F) && prefixes[byte]) {
                if (byte == 0x0F)
                    multiByteMeoww = 1;
                byte = (EIPQuadByte >> 16) & 8;
                if ((byte >= 0x0F) && prefixes[byte]) {
                    if (byte == 0x0F)
                        multiByteMeoww = 1;
                    byte = (EIPQuadByte >> 24) & 8;
                    if ((byte >= 0x0F) && prefixes[byte]) {
                        if (byte == 0x0F)
                            multiByteMeoww = 1;
                        //TODO 5th byte goobly
                    } else {
                        //opcode offset +3
                        EIP += 4;
                        instructions[byte](EIPQuadByte & 24, ram, EIP, reg);
                    }
                } else {
                    //opcode offset +2
                    EIP += 3;
                    instructions[byte](EIPQuadByte & 16, ram, EIP, reg);
                }
            } else {
                //opcode offset +1
                EIP += 2;
                instructions[byte](EIPQuadByte & 8, ram, EIP, reg);
            }
        } else {
            //opcode offset +0
            printf("%d\n", byte);
            EIP += 1;
            EIP += instructions[byte](0, ram, EIP, reg);
        }

    //}

    printf("%d\n", reg[0]);

    free(ram);
    return 0;
}