#include "main.h"

/**
 * _putnbr_unsigned_base - print an unsigned long in a given base
 * @n: number to print
 * @base: base (2..16)
 * @upper: 1 for uppercase hex digits, 0 for lowercase
 * Return: chars printed
 */
int _putnbr_unsigned_base(unsigned long n, unsigned int base, int upper)
{
	char buf[64];
	const char *digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
	int i = 0, count = 0;

	if (base < 2 || base > 16)
		return (0);

	if (n == 0)
	{
		_putchar('0');
		return (1);
	}

	while (n != 0)
	{
		buf[i++] = digits[n % base];
		n /= base;
	}
	while (i--)
		count += _putchar(buf[i]);

	return (count);
}

/**
 * _putnbr_signed - print a signed long in base 10 (INT/LONG_MIN safe)
 * @n: number to print
 * Return: chars printed
 */
int _putnbr_signed(long n)
{
	int count = 0;
	unsigned long u;

	if (n < 0)
	{
		count += _putchar('-');
		u = (unsigned long)(-(n + 0L));
	}
	else
	{
		u = (unsigned long)n;
	}

	return (count + _putnbr_unsigned_base(u, 10, 0));
}
