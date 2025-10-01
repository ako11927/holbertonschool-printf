#include "main.h"

/**
 * _putchar - buffered putchar
 * @c: character
 * Return: 1 on success, -1 on error
 */
int _putchar(char c)
{
	static char buf[1024];
	static int idx;
	int r = 1;

	if (c == -1 || idx >= 1023)
	{
		r = write(1, buf, idx);
		idx = 0;
	}
	if (c != -1)
		buf[idx++] = c;
	return (r == -1 ? -1 : 1);
}

/**
 * _putchar_flush - flush the internal buffer
 * Return: 1 (or -1 on error)
 */
int _putchar_flush(void)
{
	return (_putchar(-1));
}

/**
 * print_char - print a single character (legacy helper)
 * @ap: va_list*
 * Return: chars written or -1
 */
int print_char(va_list *ap)
{
	char c = (char)va_arg(*ap, int);

	return (_putchar(c) == -1 ? -1 : 1);
}

/**
 * putnchar - write character @c exactly @n times
 * @c: character to repeat
 * @n: count
 * Return: number written, or -1 on error
 */
int putnchar(char c, int n)
{
	int i, wrote = 0;

	if (n <= 0)
		return (0);

	for (i = 0; i < n; i++)
	{
		if (_putchar(c) == -1)
			return (-1);
		wrote++;
	}
	return (wrote);
}
