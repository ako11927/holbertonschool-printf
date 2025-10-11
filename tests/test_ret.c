// test_ret.c
#include "main.h"
#include <stdio.h>

int main(void) {
    int a;
    a = _printf("%+d%+d", 1024, 1024);
    printf("\n[ret=%d]\n", a); /* system printf shows what your _printf returned */
    return 0;
}
