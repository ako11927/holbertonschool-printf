#include "main.h"
#include <stdarg.h>

/**
 * handle_spec - build a minimal fmt_t from a spec and print it
 * @c: conversion character
 * @ap: varargs (by value)
 * Return: chars printed or -1
 *
 * We pass the address of @ap to match the declaration
 *   int print_formatted(const fmt_t *f, va_list *ap);
 * On some GNU systems va_list has a special underlying type; the cast
 * keeps the compiler happy under -Werror.
 */
int handle_spec(char c, va_list ap)
{
	fmt_t f;

	f.f_plus = 0;
	f.f_space = 0;
	f.f_hash = 0;
	f.f_zero = 0;
	f.f_minus = 0;
	f.width = 0;
	f.precision = -1;
	f.length = 0;
	f.spec = c;

	return (print_formatted(&f, (va_list *)&ap));
}
