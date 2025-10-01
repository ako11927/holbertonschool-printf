#include "main.h"

/**
 * print_pointer - prints a pointer value like standard printf
 * @p: pointer
 *
 * Return: number of chars printed or -1 on error
 */
int print_pointer(const void *p)
{
	unsigned long v;
	int count = 0, k;

	if (p == NULL)
		return (print_string("(nil)"));

	/* 0x prefix */
	if (_putchar('0') < 0 || _putchar('x') < 0)
		return (-1);
	count += 2;

	v = (unsigned long)p;
	k = print_base(v, 16, 0);
	if (k < 0)
		return (-1);
	return (count + k);
}
