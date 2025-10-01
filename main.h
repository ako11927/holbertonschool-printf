#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <stddef.h>

/**
 * struct fmt_s - parsed printf fields
 * @f_plus: '+' flag
 * @f_space: ' ' flag
 * @f_hash: '#' flag
 * @f_zero: '0' flag
 * @f_minus: '-' flag
 * @width: field width (-1 none, -2 means read '*' from args)
 * @precision: precision (-1 none, -2 means read '*' from args)
 * @length: 0 none, 1 'h', 2 'l'
 * @spec: conversion specifier character
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

/* core output */
int _putchar(char c);
void _putchar_flush(void);
int putnchar(char c, int n);

/* project helpers already in repo */
int print_S(const char *s);
int print_rev(const char *s);
int print_rot13(const char *s);
int print_base(unsigned long n, int base, int uppercase);
int print_int(long n);
int print_uint(unsigned long n);
int print_pointer(const void *p);
int print_char(char c);
int print_string(const char *s);

/* parsing + dispatch */
int parse_format(const char *fmt, int *i, fmt_t *out);
int handle_spec(const char *fmt, int *i, va_list *ap);
int print_formatted(const fmt_t *f, va_list *ap);

/* small utilities used by the formatters */
size_t strnlen_prec(const char *s, int prec);
int utoa_base(unsigned long n, int base, int upper, char *buf);

#endif /* MAIN_H */
