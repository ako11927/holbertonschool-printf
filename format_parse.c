#include "main.h"

/* C89 helpers */
static int is_digit(int c) { return (c >= '0' && c <= '9'); }

/*
 * parse_format - parse after a '%' at fmt[*i]
 * Sets *i to the index AFTER the conversion spec
 * Returns 1 on success, -1 on error (e.g., premature end)
 */
int parse_format(const char *fmt, int *i, fmt_t *out, va_list *ap)
{
	int j, c, val;
	fmt_t f;

	j = *i + 1;
	f.f_plus = f.f_space = f.f_hash = f.f_zero = f.f_minus = 0;
	f.width = -1;
	f.precision = -1;
	f.length = 0;
	f.spec = '\0';

	/* ---- flags ---- */
	for (; (c = fmt[j]) == '+' || c == ' ' || c == '#' || c == '0' || c == '-'; j++)
	{
		if (c == '+') f.f_plus = 1;
		else if (c == ' ') f.f_space = 1;
		else if (c == '#') f.f_hash = 1;
		else if (c == '0') f.f_zero = 1;
		else if (c == '-') f.f_minus = 1;
	}

	/* ---- width ---- */
	if (fmt[j] == '*')
	{
		f.width = va_arg(*ap, int);
		j++;
	}
	else if (is_digit(fmt[j]))
	{
		val = 0;
		while (is_digit(fmt[j])) { val = val * 10 + (fmt[j] - '0'); j++; }
		f.width = val;
	}

	/* ---- precision ---- */
	if (fmt[j] == '.')
	{
		j++;
		if (fmt[j] == '*') { f.precision = va_arg(*ap, int); j++; }
		else
		{
			val = 0; /* no digits => precision==0 */
			while (is_digit(fmt[j])) { val = val * 10 + (fmt[j] - '0'); j++; }
			f.precision = val;
		}
	}

	/* ---- length ---- */
	if (fmt[j] == 'h') { f.length = 1; j++; }
	else if (fmt[j] == 'l') { f.length = 2; j++; }

	/* ---- spec ---- */
	f.spec = fmt[j];
	if (f.spec == '\0')
		return (-1);

	*out = f;
	*i = j + 1;
	return (1);
}
