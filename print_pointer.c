#include "main.h"

/**
 * print_pointer - print a pointer value like 0x...
 * @p: pointer
 * Return: chars printed or -1 on error
 */
int print_pointer(const void *p)
{
	unsigned long v = (unsigned long)p;
	int count = 0;

	if (_putchar('0') == -1 || _putchar('x') == -1)
		return (-1);
	count += 2;

	return (count + print_base(v, 16, 0));
}
