#include "main.h"

/**
 * _putchar - buffered character writer
 * @c: character to write; pass -1 to flush buffer
 * Return: 1 on success, -1 on error
 */
int _putchar(char c)
{
	static char buf[1024];
	static int idx;
	int r = 1;

	if (c == -1 || idx >= 1023)
	{
		if (idx)
			r = write(1, buf, idx);
		idx = 0;
	}
	if (c != -1)
		buf[idx++] = c;
	return (r == -1 ? -1 : 1);
}

/**
 * _putchar_flush - flush internal buffer
 */
void _putchar_flush(void)
{
	(void)_putchar(-1);
}

/**
 * print_char - legacy single-character printer
 * @c: character
 * Return: chars written or -1 on error
 */
int print_char(char c)
{
	return (_putchar(c) == -1 ? -1 : 1);
}

/**
 * putnchar - print @c exactly @n times
 * @c: character to write
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
