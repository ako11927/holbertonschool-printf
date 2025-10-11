#include "main.h"

/**
 * handle_spec - parse and print one directive starting at '%' in fmt[*i]
 * @fmt: format string
 * @i: index of '%' on entry; set to the char after the spec on return
 * @ap: va_list
 * Return: chars printed on success; -1 on error
 */
int handle_spec(const char *fmt, int *i, va_list *ap)
{
	fmt_t f;
	int r;

	if (parse_format(fmt, i, &f, ap) == -1)
		return (-1);

	r = print_formatted(&f, ap);
	return (r);
}
