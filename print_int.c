#include "main.h"

/**
 * print_int - prints a signed integer
 * @n: number to print
 *
 * Description: Prints a signed decimal number using recursion.
 * Handles negative values, including INT_MIN.
 *
 * Return: number of characters printed, or -1 on error
 */
int print_int(long n)
{
    int count = 0;
    unsigned long u;

    if (n < 0)
    {
        if (_putchar('-') < 0)
            return (-1);
        count++;
        u = (unsigned long)(-n);
    }
    else
    {
        u = (unsigned long)n;
    }

    if (u / 10)
    {
        int k = print_int((long)(u / 10));

        if (k < 0)
            return (-1);
        count += k;
    }
    if (_putchar((char)('0' + (u % 10))) < 0)
        return (-1);
    return (count + 1);
}
