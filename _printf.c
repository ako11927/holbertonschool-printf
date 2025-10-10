/* _printf.c — reference structure */
#include "main.h"

int _printf(const char *format, ...)
{
    va_list ap;
    int i = 0, out = 0;
    int k;

    if (!format)
        return (-1);

    va_start(ap, format);

    while (format[i] != '\0')
    {
        if (format[i] != '%')
        {
            if (_putchar(format[i]) == -1)
            {
                va_end(ap);
                return (-1);
            }
            out++;
            i++;
            continue;
        }

        /* we saw '%': parse specifier that starts at i+1 */
        i++; /* skip '%' */

        /* Your project-specific parser should fill a fmt_t and
           tell you where the specifier ends (the index after it). */
        fmt_t f;
        int next = format_parse(format, i, &f); /* name may differ in your code */
        if (next < 0)
        {
            va_end(ap);
            return (-1);
        }

        k = print_formatted(&f, &ap);   /* <- MUST return how many chars it printed */
        if (k < 0)
        {
            va_end(ap);
            return (-1);
        }
        out += k;            /* <- CRITICAL: add to total */
        i = next;            /* <- CRITICAL: advance i to the end of this specifier */
    }

    va_end(ap);
    return (out);
}
