#include "main.h"

/**
 * handle_spec - dispatches a format specifier
 * @sp: specifier character
 * @ap: pointer to the va_list
 *
 * Return: chars printed, or -1 on error
 */
int handle_spec(char sp, va_list *ap)
{
	if (sp == 'c')
		return (print_char((char)va_arg(*ap, int)));
	if (sp == 's')
		return (print_string(va_arg(*ap, const char *)));
	if (sp == '%')
		return (_putchar('%'));
	if (sp == 'd' || sp == 'i')
		return (print_int((long)va_arg(*ap, int)));

	if (_putchar('%') < 0)
		return (-1);
	if (_putchar(sp) < 0)
		return (-1);
	return (2);
}
