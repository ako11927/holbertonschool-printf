#include "main.h"

/**
 * _printf - produces output according to a format
 * @format: format string
 *
 * Return: number of characters printed, or -1 on error
 */
int _printf(const char *format, ...)
{
	va_list ap;
	int i, count = 0, k;

	if (format == NULL)
		return (-1);

	va_start(ap, format);
	for (i = 0; format[i] != '\0'; i++)
	{
		if (format[i] != '%')
		{
			if (_putchar(format[i]) < 0)
			{
				count = -1;
				break;
			}
			count++;
			continue;
		}
		i++;
		if (format[i] == '\0')
		{
			count = -1;
			break;
		}
		k = handle_spec(format[i], &ap);
		if (k < 0)
		{
			count = -1;
			break;
		}
		count += k;
	}
	va_end(ap);
	_putchar_flush();
	return (count);
}
