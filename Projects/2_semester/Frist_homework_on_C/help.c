#include <stdio.h>
#include "stdint.h"

typedef unsigned long long ull_t;
const ull_t SIZE = (1UL << 60);

int main() {
    printf("%llu", SIZE);
    //int arr[SIZE] = {};
    return 0;
}