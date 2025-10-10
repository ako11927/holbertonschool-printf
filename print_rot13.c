#include "main.h"

int print_rot13(const char *s)
{
	int out = 0;
	char c;

	if (s == 0)
		s = "(null)";

	while (*s)
	{
		c = *s++;
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		{
			char base = (c >= 'a') ? 'a' : 'A';
			c = (char)(base + ((c - base + 13) % 26));
		}
		if (_putchar(c) == -1)
			return (-1);
		out++;
	}
	return (out);
}
