#include "main.h"

int _putchar(char c)
{
	int r = write(1, &c, 1);
	return (r == 1) ? 1 : -1;
}
