#include "main.h"

/**
 * _printf - project printf
 * @format: format string
 * Return: chars printed, or -1 on error
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
				_putchar_flush();
				return (-1);
			}
			out++;
			i++;
			continue;
		}

		/* "%%" */
		if (format[i + 1] == '%')
		{
			if (_putchar('%') == -1)
			{
				va_end(ap);
				_putchar_flush();
				return (-1);
			}
			out++;
			i += 2;
			continue;
		}

		/* delegate; handle_spec moves i past the conversion */
		k = handle_spec(format, &i, &ap);
		if (k == -1)
		{
			va_end(ap);
			_putchar_flush();
			return (-1);
		}
		out += k;
	}

	va_end(ap);
	/* IMPORTANT: flush our internal buffered writer so the checker sees output */
	_putchar_flush();
	return (out);
}
