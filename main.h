#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

/* 1KB buffered writer */
int _putchar(char c);
void _putchar_flush(void);

/* public API */
int _printf(const char *format, ...);

/* basic printers already in your repo */
int print_char(char c);
int print_string(const char *s);
int print_int(long n);
int print_uint(unsigned long n);
int print_base(unsigned long n, int base, int uppercase);
int print_pointer(const void *p);
int print_S(const char *s);
int print_rev(const char *s);
int print_rot13(const char *s);

/* format descriptor for flags/width/precision/length */
/**
 * struct fmt_s - parsed formatting options
 * @f_plus: '+' flag
 * @f_space: ' ' (space) flag
 * @f_hash: '#' flag
 * @f_zero: '0' flag
 * @f_minus: '-' flag
 * @width: field width, or -1 if not specified
 * @precision: precision, or -1 if not specified
 * @length: length modifier ('l', 'h', or 0)
 * @spec: conversion specifier character
 */
typedef struct fmt_s
{
	int f_plus;
	int f_space;
	int f_hash;
	int f_zero;
	int f_minus;
	int width;       /* -1 if not set */
	int precision;   /* -1 if not set */
	char length;     /* 'l', 'h', or 0 */
	char spec;       /* final conversion char */
} fmt_t;

/* parser + dispatcher */
int parse_format(const char *fmt, int *i, fmt_t *out);
int print_formatted(const fmt_t *f, va_list *ap);

/* small util */
int putnchar(char c, int n);

#endif /* MAIN_H */
