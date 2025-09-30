#include <stdarg.h>
#include <unistd.h>

/**
 * print_char - prints a character
 * @args: va_list containing the char
 *
 * Return: number of characters printed
 */
int print_char(va_list args)
{
	char c = (char)va_arg(args, int);

	write(1, &c, 1);
	return (1);
}

/**
 * print_string - prints a string
 * @args: va_list containing the string
 *
 * Return: number of characters printed
 */
int print_string(va_list args)
{
	char *str;
	int count = 0;

	str = va_arg(args, char *);
	if (!str)
		str = "(null)";
	while (*str)
	{
		write(1, str++, 1);
		count++;
	}
	return (count);
}

/**
 * handle_format - handles format specifiers
 * @p: current format character
 * @args: list of arguments
 *
 * Return: number of characters printed
 */
int handle_format(char p, va_list args)
{
	if (p == 'c')
		return (print_char(args));
	else if (p == 's')
		return (print_string(args));
	else if (p == '%')
	{
		write(1, "%", 1);
		return (1);
	}
	else
	{
		write(1, "%", 1);
		write(1, &p, 1);
		return (2);
	}
}

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

	if (!format)
		return (-1);

	va_start(args, format);

	for (p = format; *p; p++)
	{
		if (*p == '%')
		{
			p++;
			if (!*p)
			{
				va_end(args);
				return (-1);
			}
			count += handle_format(*p, args);
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
