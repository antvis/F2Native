#include "WilkinsonExtended.h"
#include <stdio.h>

int main() {
    double outlmin, outlmax, outlstep;
    int rst = easy_wilk_ext(0, 3005, 3, -1, &outlmin, &outlmax, &outlstep);
    printf("%d %.2f %.2f %.2f \n", rst, outlmin, outlmax, outlstep);

    return 0;
}
