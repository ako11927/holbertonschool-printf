#include "main.h"

/**
 * print_base - print unsigned long in base (2..16)
 * @n: number
 * @base: base
 * @uppercase: non-zero => A..F, else a..f
 * Return: chars printed or -1
 */
int print_base(unsigned long n, int base, int uppercase)
{
	char stack[64];
	const char *d = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
	int top = 0, out = 0;

	if (base < 2 || base > 16)
		return (0);

	if (n == 0)
		stack[top++] = '0';
	else {
		while (n) {
			stack[top++] = d[n % (unsigned)base];
			n /= (unsigned)base;
		}
	}

	while (top) {
		if (_putchar(stack[--top]) == -1)
			return (-1);
		out++;
	}
	return (out);
}
