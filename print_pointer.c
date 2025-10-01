#include "main.h"

/**
 * print_pointer - print pointer value as 0x...
 * @p: pointer
 * Return: chars printed or -1 on error
 */
int print_pointer(const void *p)
{
	unsigned long v = (unsigned long)p;
	int n = 0, k;

	if (_putchar('0') == -1 || _putchar('x') == -1)
		return (-1);
	n += 2;

	k = print_base(v, 16, 0);
	if (k == -1)
		return (-1);
	return (n + k);
}
