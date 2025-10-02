#include "main.h"
#include <unistd.h>

#define BUF_SIZE 1024

static char buf[BUF_SIZE];
static int idx;

/**
 * _putchar - buffered putchar to stdout
 * @c: character
 * Return: 1 on success, -1 on write failure
 */
int _putchar(char c)
{
	ssize_t r;

	if (idx >= BUF_SIZE) {
		r = write(1, buf, idx);
		if (r != idx)
			return (-1);
		idx = 0;
	}
	buf[idx++] = c;
	return (1);
}

/**
 * _putchar_flush - flush internal buffer
 */
void _putchar_flush(void)
{
	if (idx > 0) {
		(void)write(1, buf, idx);
		idx = 0;
	}
}

/**
 * putnchar - print the same char n times
 * @c: char
 * @n: count (<= 0 prints nothing)
 * Return: written count or -1 on error
 */
int putnchar(char c, int n)
{
	int i;

	for (i = 0; i < n; i++)
		if (_putchar(c) == -1)
			return (-1);
	return (n < 0 ? 0 : n);
}
