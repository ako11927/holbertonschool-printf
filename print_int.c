#include "main.h"

int print_int(int n)
{
    unsigned int u;
    int k = 0;

    if (n < 0) {
        if (_putchar('-') == -1) return -1;
        k++;
        /* careful with INT_MIN: cast before negating */
        u = (unsigned int)(-(long)n);
    } else {
        u = (unsigned int)n;
    }
    {
        int t = print_base((unsigned long)u, 10, 0);
        if (t == -1) return -1;
        k += t;
    }
    return k;
}
