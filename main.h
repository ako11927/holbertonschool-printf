#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>

/* _printf function prototype */
int _printf(const char *format, ...);

/* Helper functions prototypes */
int print_char(va_list args);
int print_string(va_list args);
int print_percent(va_list args);
int print_int(va_list args);
int print_number(unsigned int n);

#endif /* MAIN_H */
