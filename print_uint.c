#include "main.h"

/**
 * print_uint - prints an unsigned long in base 10
 * @n: number
 *
 * Return: chars printed or -1 on error
 */
int print_uint(unsigned long n)
{
	int count = 0;

	if (n / 10)
	{
		int k = print_uint(n / 10);

		if (k < 0)
			return (-1);
		count += k;
	}
	if (_putchar((char)('0' + (n % 10))) < 0)
		return (-1);
	return (count + 1);
}
