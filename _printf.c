#include "main.h"

/**
 * _printf - produces output according to a format
 * @format: the format string
 *
 * Description: This function supports the following format specifiers:
 * %c - character
 * %s - string
 * %% - percent sign
 * %d and %i - signed decimal integer
 * Unknown specifiers are printed as '%' followed by the character.
 *
 * Return: The number of characters printed (excluding null byte),
 * or -1 if an error occurs.
 */
int _printf(const char *format, ...)
{
    va_list ap;
    int i, count = 0;

    if (format == NULL)
        return (-1);

    va_start(ap, format);

    for (i = 0; format[i] != '\0'; i++)
    {
        if (format[i] != '%')
        {
            if (_putchar(format[i]) < 0)
            {
                count = -1;
                break;
            }
            count++;
            continue;
        }

        i++;
        if (format[i] == '\0')
        {
            count = -1;
            break;
        }

        if (format[i] == 'c')
        {
            char c = (char)va_arg(ap, int);

            if (_putchar(c) < 0)
            {
                count = -1;
                break;
            }
            count++;
        }
        else if (format[i] == 's')
        {
            const char *s = va_arg(ap, const char *);
            int j;

            if (s == NULL)
                s = "(null)";
            for (j = 0; s[j] != '\0'; j++)
            {
                if (_putchar(s[j]) < 0)
                {
                    count = -1;
                    break;
                }
                count++;
            }
            if (count < 0)
                break;
        }
        else if (format[i] == '%')
        {
            if (_putchar('%') < 0)
            {
                count = -1;
                break;
            }
            count++;
        }
        else if (format[i] == 'd' || format[i] == 'i')
        {
            int val = va_arg(ap, int);
            int k = print_int((long)val);

            if (k < 0)
            {
                count = -1;
                break;
            }
            count += k;
        }
        else
        {
            if (_putchar('%') < 0 || _putchar(format[i]) < 0)
            {
                count = -1;
                break;
            }
            count += 2;
        }
    }

    va_end(ap);
    return (count);
}
