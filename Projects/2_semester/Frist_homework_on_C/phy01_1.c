#include <stdint.h>


uint16_t
satsum(uint16_t x, uint16_t y) {
    uint16_t z = ~0;

    if (x > z - y) {
        return z;
    } else {
        return x + y;
    }
}
