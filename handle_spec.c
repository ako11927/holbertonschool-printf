#include "main.h"

/**
 * handle_spec - parse at fmt[*i] (which must point at '%') and print it
 * @fmt: format string
 * @i:   in/out index (points to '%'; will be advanced past the spec)
 * @ap:  va_list
 * Return: number of chars printed, or -1 on error
 */
int handle_spec(const char *fmt, int *i, va_list *ap)
{
	fmt_t f;
	int r;

	r = parse_format(fmt, i, &f, ap);
	if (r == -1)
		return (-1);

	return (print_formatted(&f, ap));
}
