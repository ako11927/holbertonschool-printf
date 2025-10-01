#include "main.h"
#include <unistd.h>

/* Buffered stdout writer (1024B) */
static char buf[1024];
static int idx;

/**
 * _putchar - buffer a single character to stdout
 * @c: character
 * Return: 1 on success, -1 on error
 */
int _putchar(char c)
{
	if (idx >= (int)sizeof(buf))
	{
		ssize_t r = write(1, buf, idx);

		if (r != idx)
			return (-1);
		idx = 0;
	}
	buf[idx++] = c;
	return (1);
}

/**
 * _putchar_flush - write out any buffered data
 */
void _putchar_flush(void)
{
	if (idx > 0)
	{
		(void)write(1, buf, idx);
		idx = 0;
	}
}

/**
 * putnchar - print a character repeated N times
 * @c: character
 * @n: repeat count
 * Return: chars printed or -1 on error
 */
int putnchar(char c, int n)
{
	int i, out = 0;

	for (i = 0; i < n; i++)
	{
		if (_putchar(c) == -1)
			return (-1);
		out++;
	}
	return (out);
}

/**
 * print_char - print one character
 * @c: character
 * Return: 1 on success, -1 on error
 */
int print_char(char c)
{
	return (_putchar(c) == -1 ? -1 : 1);
}
