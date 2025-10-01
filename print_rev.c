#include "main.h"

/**
 * print_rev - prints a reversed string
 * @s: string (prints "(null)" if NULL)
 *
 * Return: chars printed or -1 on error
 */
int print_rev(const char *s)
{
	int len = 0, i, count = 0;

	if (s == NULL)
		s = "(null)";
	while (s[len] != '\0')
		len++;
	for (i = len - 1; i >= 0; i--)
	{
		if (_putchar(s[i]) < 0)
			return (-1);
		count++;
	}
	return (count);
}
