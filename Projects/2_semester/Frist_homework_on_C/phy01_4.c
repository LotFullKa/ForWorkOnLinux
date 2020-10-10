#include <stdint.h>


enum {
    LIMIT = (uint32_t)1 << 24,
    GOOD_VALUES = 23
};

uint32_t
my_pow(uint32_t num, uint32_t *mask){
    *mask = 0;
    uint32_t i = 1;
    while (i < num) {
        i = i << 1;
        *mask = (*mask << 1) | 1;

        if (i >= ((uint32_t)1 << 31)) {
            *mask = *mask >> 1;
            return i;
        }
    }
    *mask = *mask >> 1;
    return i >> 1;
}


extern int
check_int(uint32_t u32) {
    uint32_t
    mask,
    exp = my_pow(u32, &mask);

    if (exp < LIMIT) {
        return 1;
    }
    else {
        mask = (mask >> GOOD_VALUES);
        if ((u32 & mask) == 0)
            return 1;
        else {
            return 0;
        }
    }
}
