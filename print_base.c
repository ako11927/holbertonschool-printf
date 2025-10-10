#include "main.h"

int print_base(unsigned long v, int base, int uppercase)
{
	char buf[64];
	int bi = 0;
	int out = 0;

	if (base < 2 || base > 16)
		return (-1);

	if (v == 0)
		buf[bi++] = '0';
	while (v > 0)
	{
		int d = (int)(v % (unsigned long)base);

		if (d < 10)
			buf[bi++] = (char)('0' + d);
		else
			buf[bi++] = (char)((uppercase ? 'A' : 'a') + (d - 10));
		v /= (unsigned long)base;
	}

	while (bi--)
	{
		if (_putchar(buf[bi]) == -1)
			return (-1);
		out++;
	}
	return (out);
}
