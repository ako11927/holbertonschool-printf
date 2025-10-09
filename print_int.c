#include "main.h"

/**
 * print_int - print a signed int in base 10
 * @n: value
 * Return: number of characters printed, or -1 on error
 */
int print_int(int n)
{
	long v = n;
	int out = 0;

	if (v < 0)
	{
		if (_putchar('-') == -1)
			return (-1);
		out++;
		v = -v;
	}
	/* project already has print_base(unsigned long, base, uppercase) */
	{
		int k = print_base((unsigned long)v, 10, 0);
		if (k == -1)
			return (-1);
		out += k;
	}
	return (out);
}
