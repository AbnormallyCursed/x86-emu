#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "x86_types.h"

static int add(register bit32 prefix, gooblyram *ram, register bit64 RIP, bit64 reg[8]) {
    if (prefix != 0) {        
        if ((prefix & 8 == 0x48) || ((prefix >> 8) & 8 == 0x48) || ((prefix >> 16) & 8 == 0x48) || ((prefix >> 24) & 8 == 0x48)) { // TODO: alternative benchmark test memory vs instruction
            // REX.W
            
        }
    } else {
        printf("meoowwwwwwwwwwwww %d\n", ram[RIP]);
        reg[0] += ram[RIP];
        return RIP++;
    }
}

static int (*instructions[])(bit32, gooblyram*, bit64, bit64[8]) = {add};