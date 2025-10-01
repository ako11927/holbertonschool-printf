#include "main.h"

/**
 * print_string - prints a C string (NULL -> "(null)")
 * @ap: pointer to va_list with the string argument
 *
 * Return: number of characters printed (or -1 on error)
 */
int print_string(va_list *ap)
{
	char *s = va_arg(*ap, char *);
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
