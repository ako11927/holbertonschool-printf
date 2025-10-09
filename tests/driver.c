#include "main.h"
#include <stdlib.h> /* strtol */

/* Tiny driver: argv[1] is the format;
 * up to 6 numeric args parsed with strtol (base 0 to allow 0x, 0, etc.) */
int main(int argc, char **argv)
{
    const char *fmt;
    long a[6];
    int i, numi;

    if (argc < 3)
        return 1;

    fmt = argv[1];
    numi = argc - 2;
    if (numi > 6)
        numi = 6;

    for (i = 0; i < 6; i++)
        a[i] = 0;

    for (i = 0; i < numi; i++)
        a[i] = strtol(argv[2 + i], (char **)0, 0);

    _printf(fmt, a[0], a[1], a[2], a[3], a[4], a[5]);
    _putchar('\n');
    _putchar_flush();
    return 0;
}
