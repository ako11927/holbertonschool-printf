#include "main.h"
#include <unistd.h>

/* 1KB buffered writer for stdout */

static char buf[1024];
static int idx;

/**
 * _putchar - buffer a single char to stdout
 * @c: character
 * Return: 1 on success, -1 on write error
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
 * _putchar_flush - flush pending buffered output
 */
void _putchar_flush(void)
{
	if (idx > 0)
	{
		/* ignore short writes here; project callers check return values */
		(void)write(1, buf, idx);
		idx = 0;
	}
}

/**
 * putnchar - print a char repeated n times
 * @c: character to repeat
 * @n: times
 * Return: number printed or -1 on error
 */
int putnchar(char c, int n)
{
	int i, cnt = 0;

	for (i = 0; i < n; i++)
	{
		if (_putchar(c) == -1)
			return (-1);
		cnt++;
	}
	return (cnt);
}

/**
 * print_char - write one character
 * @c: character
 * Return: 1 on success, -1 on error
 */
int print_char(char c)
{
	if (_putchar(c) == -1)
		return (-1);
	return (1);
}
