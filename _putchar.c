#include "main.h"
#include <unistd.h>

#define BUF_SZ 1024

static char buf[BUF_SZ];
static int bidx;

int _putchar(char c)
{
	buf[bidx++] = c;
	if (bidx == BUF_SZ)
	{
		if (write(1, buf, bidx) != bidx)
			return (-1);
		bidx = 0;
	}
	return (1);
}

void _putchar_flush(void)
{
	if (bidx > 0)
	{
		(void)write(1, buf, bidx);
		bidx = 0;
	}
}

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
