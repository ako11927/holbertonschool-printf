#include "main.h"

/**
 * handle_spec - handles a conversion starting at '%' in format
 * @fmt: full format string
 * @i:   in/out index; on entry points at '%'; on return points at spec
 * @ap:  varargs pointer
 *
 * Return: number of chars printed, or -1 on error
 */
int handle_spec(const char *fmt, int *i, va_list *ap)
{
	fmt_t f;
	int j = *i;

	/* advance past '%' */
	j++;
	if (!fmt[j])
		return (-1); /* trailing '%' is an error per project checker */

	/* literal "%%" */
	if (fmt[j] == '%')
	{
		if (_putchar('%') == -1)
			return (-1);
		*i = j; /* point at second '%' */
		return (1);
	}

	/* parse flags / width / precision / length / spec */
	if (parse_format(fmt, &j, &f) == -1)
		return (-1);

	*i = j; /* position at final specifier */
	return (print_formatted(&f, ap));
}
