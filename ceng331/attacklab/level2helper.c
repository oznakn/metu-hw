#include <stdio.h>

int main(void) {
    int cookie = 0x2e0e4285;

    printf("%x\n", (cookie & (( cookie  >> 16) | 0xAD0BE000)));

    return 0;
}
