#include "main.h"

int print_S(const char *s)
{
	int out = 0;
	unsigned char c;
	char hex[2];
	int d;

	if (s == 0)
		s = "(null)";

	while (*s != '\0')
	{
		c = (unsigned char)*s++;
		if (c >= 32 && c < 127)
		{
			if (_putchar((char)c) == -1)
				return (-1);
			out++;
			continue;
		}
		if (_putchar('\\') == -1)
			return (-1);
		out++;
		if (_putchar('x') == -1)
			return (-1);
		out++;

		d = (c >> 4) & 0xF;
		hex[0] = (d < 10) ? (char)('0' + d) : (char)('A' + d - 10);
		d = c & 0xF;
		hex[1] = (d < 10) ? (char)('0' + d) : (char)('A' + d - 10);

		if (_putchar(hex[0]) == -1 || _putchar(hex[1]) == -1)
			return (-1);
		out += 2;
	}
	return (out);
}
