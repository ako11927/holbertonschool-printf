#ifndef PRINTF_MAIN_H
#define PRINTF_MAIN_H

#include <stdarg.h>
#include <stddef.h>

/* public entry */
int _printf(const char *format, ...);

/* buffered I/O already in your project */
int _putchar(char c);
void _putchar_flush(void);
int putnchar(char c, int n);

/**
 * struct fmt_s - parsed format descriptor
 * @f_plus:   '+' flag
 * @f_space:  ' ' flag
 * @f_hash:   '#' flag
 * @f_zero:   '0' flag
 * @f_minus:  '-' flag
 * @width:    field width (-1 if not set)
 * @precision: precision (-1 if not set)
 * @length:   0 none, 1 = 'h', 2 = 'l'
 * @spec:     conversion specifier
 */
typedef struct fmt_s
{
	int f_plus;
	int f_space;
	int f_hash;
	int f_zero;
	int f_minus;
	int width;
	int precision;
	int length;
	char spec;
} fmt_t;

/* core */
int parse_format(const char *fmt, int *i, fmt_t *out, va_list *ap);
int print_formatted(const fmt_t *f_in, va_list *ap);
int handle_spec(char sp, va_list *ap);

/* helpers that already exist in your repo */
size_t strnlen_prec(const char *s, int prec);
int print_S(const char *s);
int print_rev(const char *s);
int print_rot13(const char *s);
int print_base(unsigned long v, int base, int uppercase);
int print_string(const char *s);
int print_pointer(const void *p);

#endif /* PRINTF_MAIN_H */
