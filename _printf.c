#include "main.h"

/**
 * _printf - minimal printf clone for the project
 * @format: format string
 * Return: number of characters written, or -1 on error
 */
int _printf(const char *format, ...)
{
	va_list ap;
	int out = 0, i = 0;

	if (!format)
		return (-1);

	va_start(ap, format);

	while (format[i])
	{
		if (format[i] != '%')
		{
			if (_putchar(format[i]) == -1) { va_end(ap); return (-1); }
			out++;
			i++;
			continue;
		}

		/* "%%" fast path */
		if (format[i + 1] == '%')
		{
			if (_putchar('%') == -1) { va_end(ap); return (-1); }
			out++;
			i += 2;
			continue;
		}

		/* Delegate: parse/print starting at '%', advance i inside */
		{
			int added = handle_spec(format, &i, &ap);
			if (added < 0) { va_end(ap); return (-1); }
			out += added;
		}
	}

	va_end(ap);
	return (out);
}
