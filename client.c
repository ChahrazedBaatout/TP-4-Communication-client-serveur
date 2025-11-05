#include <stdio.h>
#include "segdef.h"

int main(void) {
    init_rand();
    long num = getrand();
    printf("Generated randomly number : %ld \n", num);
    return 0;
}