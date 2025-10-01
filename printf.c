#include "main.h"
#include <stdarg.h>

/* _printf implementation */
int _printf(const char *format, ...)
{
    va_list args;
    int count = 0;
    int i = 0;

    if (!format)
        return -1;

    va_start(args, format);

    while (format[i])
    {
        if (format[i] == '%')
        {
            i++;
            if (format[i] == 'c')
                count += print_char(args);
            else if (format[i] == 's')
                count += print_string(args);
            else if (format[i] == '%')
                count += print_percent(args);
            else if (format[i] == 'd' || format[i] == 'i')
                count += print_int(args);
            else
                count += write(1, &format[i], 1); /* unknown specifier prints as is */
        }
        else
        {
            count += write(1, &format[i], 1);
        }
        i++;
    }

    va_end(args);
    return count;
}
