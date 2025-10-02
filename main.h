#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>

/* Core */
int _printf(const char *format, ...);

/* helpers */
int _putchar(char c);
int print_char(char c);
int print_string(const char *s);
int print_int(long n);
int handle_spec(char sp, va_list *ap);

#endif /* MAIN_H */
