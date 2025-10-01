#include "main.h"

/**
 * _printf - minimal printf clone for project
 * @format: format string
 * Return: number of characters printed, or -1 on error
 */
int _printf(const char *format, ...)
{
	va_list ap;
	int i = 0, count = 0;

	if (!format)
		return (-1);

	va_start(ap, format);

	while (format[i])
	{
		if (format[i] != '%')
		{
			if (_putchar(format[i]) == -1)
			{
				count = -1;
				break;
			}
			count++;
			i++;
			continue;
		}

		/* '%' dispatch */
		i++; /* look at the char after '%' */

		/* lone '%' at end is an error (project expectation) */
		if (format[i] == '\0')
		{
			count = -1;
			break;
		}

		/* literal percent: "%%" -> print single '%' */
		if (format[i] == '%')
		{
			if (_putchar('%') == -1)
			{
				count = -1;
				break;
			}
			count++;
			i++;
			continue;
		}

		/* step back so handle_spec sees the '%' */
		i--;

		{
			int added = handle_spec(format, &i, &ap);

			if (added == -1)
			{
				count = -1;
				break;
			}
			count += added;
		}
	}

	va_end(ap);
	return (count);
}

