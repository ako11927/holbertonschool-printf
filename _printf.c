#include "main.h"

/**
 * _printf - minimal printf clone
 * @format: format string
 *
 * Return: number of characters printed, or -1 on error
 */
int _printf(const char *format, ...)
{
	va_list ap;
	int i, added, count = 0;

	if (!format)
		return (-1);

	va_start(ap, format);

	for (i = 0; format[i]; i++)
	{
		if (format[i] != '%')
		{
			if (_putchar(format[i]) == -1)
			{
				va_end(ap);
				return (-1);
			}
			count++;
			continue;
		}

		/* '%' branch: delegate entirely to handle_spec */
		added = handle_spec(format, &i, &ap);
		if (added < 0)
		{
			va_end(ap);
			return (-1);
		}
		count += added;
		/* IMPORTANT: do NOT print format[i] here.
		 * handle_spec advanced i to the spec and already printed output. */
	}

	va_end(ap);
	return (count);
}
