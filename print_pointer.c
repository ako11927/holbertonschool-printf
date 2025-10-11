#include "main.h"
#include <stddef.h>      /* for NULL */
#include <stdint.h>      /* for uintptr_t (optional but cleaner) */

int print_pointer(const void *p)
{
    /* declare all variables at the top for -std=gnu89 / -pedantic */
    uintptr_t addr;
    char hex_digits[] = "0123456789abcdef";
    char rev[2 * sizeof(void *)]; /* up to 16 nibbles on 64-bit */
    char out[2 + (2 * sizeof(void *)) + 1]; /* "0x" + nibbles + NUL */
    int ri;
    int oi;
    int i;

    if (p == NULL)
        return print_string("(nil)");

    addr = (uintptr_t)p;

    /* Special-case zero just in case; prints 0x0 */
    /* Build reversed hex into rev[] */
    ri = 0;
    while (addr != (uintptr_t)0) {
        rev[ri++] = hex_digits[addr & 0xF];
        addr >>= 4;
    }

    /* Build final string "0x" + reversed digits in correct order */
    oi = 0;
    out[oi++] = '0';
    out[oi++] = 'x';
    for (i = ri - 1; i >= 0; --i)
        out[oi++] = rev[i];
    out[oi] = '\0';

    return print_string(out);
}
