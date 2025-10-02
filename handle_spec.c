#include "main.h"

/**
 * handle_spec - parse one %... conversion and print it
 * @fmt: full format string
 * @i:   in/out index (points at '%'; ends at spec char)
 * @ap:  va_list
 * Return: number of chars printed (>=0) or -1 on error
 */
int handle_spec(const char *fmt, int *i, va_list *ap)
{
	fmt_t f;
	int j = *i + 1;

	if (!fmt[j]) /* stray '%' at end => print it literally */
		return (_putchar('%') == -1 ? -1 : 1);

	/* defaults */
	f.f_plus = f.f_space = f.f_hash = f.f_zero = f.f_minus = 0;
	f.width = -1;
	f.precision = -1;
	f.length = 0;
	f.spec = 0;

	/* flags */
	for (; fmt[j]; j++) {
		if      (fmt[j] == '+') f.f_plus  = 1;
		else if (fmt[j] == ' ') f.f_space = 1;
		else if (fmt[j] == '#') f.f_hash  = 1;
		else if (fmt[j] == '0') f.f_zero  = 1;
		else if (fmt[j] == '-') f.f_minus = 1;
		else break;
	}

	/* width */
	if (fmt[j] == '*') {
		int w = va_arg(*ap, int);
		if (w < 0) { f.f_minus = 1; w = -w; }
		f.width = w;
		j++;
	} else if (fmt[j] >= '0' && fmt[j] <= '9') {
		int w = 0;
		while (fmt[j] >= '0' && fmt[j] <= '9') { w = w * 10 + (fmt[j++] - '0'); }
		f.width = w;
	}

	/* precision */
	if (fmt[j] == '.') {
		j++;
		if (fmt[j] == '*') {
			int p = va_arg(*ap, int);
			f.precision = (p < 0) ? -1 : p;
			j++;
		} else {
			int p = 0, saw = 0;
			while (fmt[j] >= '0' && fmt[j] <= '9') { p = p * 10 + (fmt[j++] - '0'); saw = 1; }
			f.precision = saw ? p : 0;
		}
	}

	/* length */
	if (fmt[j] == 'h')      { f.length = 1; j++; }
	else if (fmt[j] == 'l') { f.length = 2; j++; }

	/* spec */
	f.spec = fmt[j] ? fmt[j] : '\0';
	if (!f.spec)
		return (_putchar('%') == -1 ? -1 : 1);

	*i = j; /* give control back at spec */

	/* special-case "%%" */
	if (f.spec == '%')
		return (_putchar('%') == -1 ? -1 : 1);

	return print_formatted(&f, ap);
}
