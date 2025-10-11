#include "main.h"
#include <stdarg.h>
#include <stddef.h>

/*
 * _printf - Minimal printf clone with specifier dispatch.
 * Supported specs are handled in handle_spec().
 * Returns: total chars printed, or -1 on error.
 */
int _printf(const char *format, ...)
{
    va_list ap;
    int i;
    int count;
    int r;

    if (format == NULL)
        return -1;

    va_start(ap, format);
    count = 0;

    for (i = 0; format[i] != '\0'; i++)
    {
        if (format[i] != '%')
        {
            if (_putchar(format[i]) == -1)
            {
                va_end(ap);
                return -1;
            }
            count++;
            continue;
        }

        /* we saw '%': move to specifier */
        i++;
        if (format[i] == '\0')
        {
            /* stray '%' at end is an error (matches project checkers) */
            va_end(ap);
            return -1;
        }

        if (format[i] == '%')
        {
            /* "%%" -> print a single '%' */
            if (_putchar('%') == -1)
            {
                va_end(ap);
                return -1;
            }
            count++;
            continue;
        }

        /* delegate to your dispatcher (from handle_spec.c) */
        r = handle_spec(format[i], &ap);
        if (r < 0)
        {
            va_end(ap);
            return -1;
        }
        count += r;
    }

    /* flush the 1024-byte buffer in _putchar (task 11) */
    _putchar(0);

    va_end(ap);
    return count;
}
