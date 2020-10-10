#include <stdio.h>
#include <stdlib.h>
enum {
    BASE = 27
};

int main(int argc, char *argcv[]) {
    double x, result;
    int y, z;

    scanf("%lf%x", &x, &y);

    z = strtol(argcv[1], NULL, BASE);
    result = (x + y + z);

    printf("%0.3lf", result);
    return 0;
}
