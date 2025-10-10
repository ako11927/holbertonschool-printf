#include "main.h"

int _printf(const char *format, ...)
{
	va_list ap;
	int out = 0;
	int i, k;

	if (format == 0)
		return (-1);

	va_start(ap, format);

	i = 0;
	while (format[i] != '\0')
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
