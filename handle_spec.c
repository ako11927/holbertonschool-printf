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

	/* advanced */
	if (sp == 'u')
		return (print_uint((unsigned long)va_arg(*ap, unsigned int)));
	if (sp == 'o')
		return (print_base((unsigned long)va_arg(*ap, unsigned int), 8, 0));
	if (sp == 'x')
		return (print_base((unsigned long)va_arg(*ap, unsigned int), 16, 0));
	if (sp == 'X')
		return (print_base((unsigned long)va_arg(*ap, unsigned int), 16, 1));
	if (sp == 'b')
		return (print_base((unsigned long)va_arg(*ap, unsigned int), 2, 0));
	if (sp == 'p')
		return (print_pointer(va_arg(*ap, void *)));
	if (sp == 'S')
		return (print_S(va_arg(*ap, const char *)));
	if (sp == 'r')
		return (print_rev(va_arg(*ap, const char *)));
	if (sp == 'R')
		return (print_rot13(va_arg(*ap, const char *)));

	/* Unknown: print '%' then the char */
	if (_putchar('%') < 0)
		return (-1);
	if (_putchar(sp) < 0)
		return (-1);
	return (2);
}
