#include "main.h"

/**
 * _printf - minimal printf clone for this project
 * @format: format string
 * Return: number of characters printed, or -1 on error
 */
int _printf(const char *format, ...)
{
	va_list ap;
	int out = 0;
	int i = 0;
	int k;

	if (!format)
		return (-1);

	va_start(ap, format);

	while (format[i])
	{
		if (format[i] != '%')
		{
			if (_putchar(format[i]) == -1)
			{
				va_end(ap);
				return (-1);
			}
			out++;
			i++;
			continue;
		}

		/* "%%" fast path */
		if (format[i + 1] == '%')
		{
			if (_putchar('%') == -1)
			{
				va_end(ap);
				return (-1);
			}
			out++;
			i += 2;
			continue;
		}

		/* delegate full conversion; handle_spec advances i */
		k = handle_spec(format, &i, &ap);
		if (k == -1)
		{
			va_end(ap);
			return (-1);
		}
		out += k;
	}

	va_end(ap);
	return (out);
}
