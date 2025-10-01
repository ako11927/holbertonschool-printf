#include "main.h"

/**
 * print_string - print a C string (NULL => "(null)")
 * @s: string
 * Return: number of characters printed, or -1 on error
 */
int print_string(const char *s)
{
	int i = 0;

	if (!s)
		s = "(null)";
	while (s[i] != '\0')
	{
		if (_putchar(s[i]) == -1)
			return (-1);
		i++;
	}
	return (i);
}
