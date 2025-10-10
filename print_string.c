#include "main.h"

int print_string(const char *s)
{
	int out = 0;

	if (s == 0)
		s = "(null)";
	while (*s != '\0')
	{
		if (_putchar(*s++) == -1)
			return (-1);
		out++;
	}
	return (out);
}
