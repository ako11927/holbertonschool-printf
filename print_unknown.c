#include "main.h"
/**
 * print__unknown_spec - print an unknown specifier as "%<c>"
 * @f_in: parsed format info
 * Return: chars printed
 */
int print__unknown_spec(const fmt_t *f_in)
{
	int out = 0;

	out += _putchar('%');
	if (f_in && f_in->spec != '\0')
		out += _putchar(f_in->spec);
	return (out);
}
