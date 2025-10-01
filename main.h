#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

/* Core */
int _printf(const char *format, ...);

/* buffered writer (Task 5) */
int _putchar(char c);
void _putchar_flush(void);

/* simple printers */
int print_char(char c);
int print_string(const char *s);
int print_int(long n);

/* advanced spec helpers */
int handle_spec(char sp, va_list *ap);

/* unsigned/base helpers */
int print_uint(unsigned long n);
int print_base(unsigned long n, int base, int uppercase);
int print_pointer(const void *p);
int print_S(const char *s);
int print_rev(const char *s);
int print_rot13(const char *s);

#endif /* MAIN_H */
