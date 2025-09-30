#include <stdarg.h>
#include <unistd.h>

/**
 * _printf - prints output according to a format
 * @format: format string (supports c, s, %)
 *
 * Return: number of characters printed
 */
int _printf(const char *format, ...)
{
	va_list args;
	int count = 0;
	const char *p;
	char c;
	char *str;

	va_start(args, format);

	if (!format)
		return (-1);

	for (p = format; *p; p++)
	{
		if (*p == '%')
		{
			p++;
			if (*p == 'c')
			{
				c = (char)va_arg(args, int);
				write(1, &c, 1);
				count++;
			}
			else if (*p == 's')
			{
				str = va_arg(args, char *);
				if (!str)
					str = "(null)";
				while (*str)
				{
					write(1, str, 1);
					str++;
					count++;
				}
			}
			else if (*p == '%')
			{
				c = '%';
				write(1, &c, 1);
				count++;
			}
		}
		else
		{
			write(1, p, 1);
			count++;
		}
	}

	va_end(args);
	return (count);
}
