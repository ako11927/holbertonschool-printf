#include "main.h"

/* prints a signed long as decimal; returns number of chars printed, or -1 on error */
int print_int(long n)
{
	int count = 0;
	unsigned long u;

	if (n < 0)
	{
		if (_putchar('-') < 0)
			return (-1);
		count++;
		u = (unsigned long)(-n); /* works even for INT_MIN by promoting before negation */
	}
	else
	{
		u = (unsigned long)n;
	}

	/* print recursively into a buffer-less stream using _putchar */
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
