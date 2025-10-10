#ifndef PRINTF_MAIN_H
#define PRINTF_MAIN_H

#include <stdarg.h>
#include <stddef.h>

/* public entry */
int _printf(const char *format, ...);

/* buffered I/O */
int _putchar(char c);
void _putchar_flush(void);
int putnchar(char c, int n);

/* parsed format */
typedef struct fmt_s
{
	int f_plus;
	int f_space;
	int f_hash;
	int f_zero;
	int f_minus;
	int width;      /* -1 if not specified */
	int precision;  /* -1 if not specified */
	int length;     /* 0 none, 1 h, 2 l */
	char spec;      /* conversion */
} fmt_t;

/* core */
int parse_format(const char *fmt, int *i, fmt_t *out, va_list *ap);
int print_formatted(const fmt_t *f_in, va_list *ap);
int handle_spec(const char *fmt, int *i, va_list *ap);

/* helpers / small printers */
size_t strnlen_prec(const char *s, int prec);
int print_base(unsigned long v, int base, int uppercase);
int print_uint(unsigned int n);
int print_int(int n);
int print_pointer(void *p);
int print_string(const char *s);

/* custom */
int print_S(const char *s);
int print_rev(const char *s);
int print_rot13(const char *s);

#endif /* PRINTF_MAIN_H */

