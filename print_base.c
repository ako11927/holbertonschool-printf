#include "main.h"

/**
 * print_base - prints unsigned in given base (2..16)
 * @n: number (unsigned long to support pointers too)
 * @base: 2, 8, 10, 16
 * @uppercase: 0 for lower, 1 for upper hex letters
 *
 * Return: chars printed or -1 on error
 */
int print_base(unsigned long n, int base, int uppercase)
{
	int count = 0;
	char c;

	if (n / (unsigned long)base)
	{
		int k = print_base(n / (unsigned long)base, base, uppercase);

		if (k < 0)
			return (-1);
		count += k;
	}
	c = (char)(n % (unsigned long)base);
	if (c < 10)
		c = (char)('0' + c);
	else
		c = (char)((uppercase ? 'A' : 'a') + (c - 10));
	if (_putchar(c) < 0)
		return (-1);
	return (count + 1);
}
