#include "main.h"

int print_pointer(const void *p)
{
	unsigned long v = (unsigned long)p;
	int out = 0;

	if (p == 0)
	{
		return (print_string("(nil)"));
	}
	if (_putchar('0') == -1)
		return (-1);
	out++;
	if (_putchar('x') == -1)
		return (-1);
	out++;
	return (out + print_base(v, 16, 0));
}
