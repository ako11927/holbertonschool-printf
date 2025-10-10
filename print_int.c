#include "main.h"

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
	return (out + print_base((unsigned long)v, 10, 0));
}
