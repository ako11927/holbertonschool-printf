#include "main.h"
#include <unistd.h>
#include <stdarg.h>

/* Print a single character */
int print_char(va_list args)
{
    char c;
    int count = 0;

    c = va_arg(args, int);
    count += write(1, &c, 1);

    return count;
}

/* Print a string */
int print_string(va_list args)
{
    char *s;
    int count = 0;

    s = va_arg(args, char *);
    if (!s)
        s = "(null)";

    while (*s)
    {
        write(1, s, 1);
        count++;
        s++;
    }
    return count;
}

/* Print a percent sign */
int print_percent(va_list args)
{
    (void)args;
    return write(1, "%", 1);
}

/* Recursive function to print an unsigned int */
int print_number(unsigned int n)
{
    int count = 0;
    char c;

    if (n / 10)
        count += print_number(n / 10);

    c = n % 10 + '0';
    count += write(1, &c, 1);

    return count;
}

/* Print an integer */
int print_int(va_list args)
{
    int n;
    unsigned int num;
    int count = 0;

    n = va_arg(args, int);

    if (n < 0)
    {
        count += write(1, "-", 1);
        num = -n;
    }
    else
    {
        num = n;
    }

    count += print_number(num);

    return count;
}
