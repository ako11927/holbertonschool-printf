#include "main.h"

/**
 * print_string - prints a C string
 * @s: string to print (prints "(null)" if NULL)
 *
 * Return: number of chars printed, or -1 on error
 */
int print_string(const char *s)
{
	int i, count = 0;

	if (s == NULL)
		s = "(null)";
	for (i = 0; s[i] != '\0'; i++)
	{
		if (_putchar(s[i]) < 0)
			return (-1);
		count++;
	}
	return (count);
}
