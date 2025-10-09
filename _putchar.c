#include "main.h"
#include <unistd.h>

#define BUF_SZ 1024
static char buf[BUF_SZ];
static int  idx;

/**
 * _putchar - buffered write of a single char
 * Return: 1 on success, -1 on error
 */
int _putchar(char c)
{
	buf[idx++] = c;
	if (idx == BUF_SZ)
	{
		if (write(1, buf, idx) != idx)
			return (-1);
		idx = 0;
	}
	return (1);
}

/**
 * _putchar_flush - flush buffered output
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
 * putnchar - print character c, n times
 * Return: number written, or -1 on error
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
