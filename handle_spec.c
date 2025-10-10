#include "main.h"

/*
 * handle_spec - called when fmt[*i] == '%'
 * Advances *i past the full conversion and prints it via print_formatted().
 * Returns the number of chars written, or -1 on error.
 */
int handle_spec(const char *fmt, int *i, va_list *ap)
{
    int j = *i;
    fmt_t f;

    if (parse_format(fmt, &j, &f, ap) == -1)
        return (-1);

    *i = j; /* advance caller index to after the conversion */
    return print_formatted(&f, ap);
}
