#include "main.h"

/**
 * print_S - prints string with non-printables as \xHH (uppercase)
 * @s: string (prints "(null)" if NULL)
 *
 * Return: chars printed or -1 on error
 */
int print_S(const char *s)
{
	int i, count = 0;

	if (s == NULL)
		s = "(null)";
	for (i = 0; s[i] != '\0'; i++)
	{
		unsigned char c = (unsigned char)s[i];

		if (c < 32 || c >= 127)
		{
			int hi = (c >> 4) & 0xF, lo = c & 0xF;
			char H = (char)(hi < 10 ? '0' + hi : 'A' + (hi - 10));
			char L = (char)(lo < 10 ? '0' + lo : 'A' + (lo - 10));

			if (_putchar('\\') < 0 || _putchar('x') < 0)
				return (-1);
			if (_putchar(H) < 0 || _putchar(L) < 0)
				return (-1);
			count += 4;
		}
		else
		{
			if (_putchar((char)c) < 0)
				return (-1);
			count++;
		}
	}
	return (count);
}
