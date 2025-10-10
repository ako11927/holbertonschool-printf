#include "main.h"

int print_rev(const char *s)
{
	int out = 0;
	int len = 0;
	const char *p;

	if (s == 0)
		s = "(null)";

	p = s;
	while (*p++)
		len++;

	while (len--)
	{
		if (_putchar(s[len]) == -1)
			return (-1);
		out++;
	}
	return (out);
}
