#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>

/* buffered I/O */
int _putchar(char c);
void _putchar_flush(void);
int putnchar(char c, int n);

/* simple printers used by the dispatcher */
int print_char(char c);
int print_string(const char *s);
int print_int(int n);
int print_uint(unsigned int n);
int print_base(unsigned long n, int base, int uppercase);
int print_pointer(const void *p);
int print_rev(const char *s);
int print_rot13(const char *s);
int print_S(const char *s);

/* parsed format fields */
typedef struct fmt_s
{
	int f_plus, f_space, f_hash, f_minus, f_zero;
	int width;          /* negative means: was read as negative then normalized */
	int precision;      /* -1 => unspecified */
	int length;         /* 0, 'h'==1, 'l'==2 */
	char spec;          /* conversion specifier */
} fmt_t;

/* parser + dispatcher */
int parse_format(const char *fmt, int *i, fmt_t *out);
int print_formatted(const fmt_t *f, va_list *ap);
int handle_spec(const char *fmt, int *i, va_list *ap);

/* public entry */
int _printf(const char *format, ...);

#endif /* MAIN_H */
