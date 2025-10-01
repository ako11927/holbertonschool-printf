#include "main.h"

/* static 1KB buffer */
static char buf[1024];
static int idx;

/**
 * _putchar - buffered putchar (stores and flushes in 1KB chunks)
 * @c: character to print
 *
 * Return: 1 on success, -1 on error
 */
int _putchar(char c)
{
	if (idx >= (int)sizeof(buf))
	{
		if (write(1, buf, idx) != idx)
		{
			idx = 0;
			return (-1);
		}
		idx = 0;
	}
	buf[idx++] = c;
	return (1);
}

/**
 * _putchar_flush - Flush the internal write buffer.
 *
 * Return: 0 on success, or -1 on error.
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
 * print_char - wrapper over _putchar
 * @c: character to print
 *
 * Return: 1 on success, -1 on error
 */
int print_char(char c)
{
	return (_putchar(c));
}
