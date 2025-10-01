#include "main.h"

/**
 * print_rot13 - prints a ROT13-encoded string
 * @s: string (prints "(null)" if NULL)
 *
 * Return: chars printed or -1 on error
 */
int print_rot13(const char *s)
{
	int i, count = 0;

	if (s == NULL)
		s = "(null)";
	for (i = 0; s[i] != '\0'; i++)
	{
		char c = s[i];

		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		{
			char base = (c >= 'a') ? 'a' : 'A';
			c = (char)(base + ((c - base + 13) % 26));
		}
		if (_putchar(c) < 0)
			return (-1);
		count++;
	}
	return (count);
}
