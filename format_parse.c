#include "main.h"

/* C89 helpers (no ctype.h to keep it tiny) */
static int is_digit(int c) { return (c >= '0' && c <= '9'); }

/**
 * parse_format - parse after a '%' at fmt[*i]
 * @fmt: format string
 * @i:   in: index of '%'; out: index after the whole directive
 * @out: filled fmt_t
 * @ap:  va_list (for * width/precision)
 * Return: 1 on success, -1 on premature end / invalid
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

	/* flags */
	while (fmt[j] == '+' || fmt[j] == ' ' || fmt[j] == '#' ||
	       fmt[j] == '0' || fmt[j] == '-')
	{
		c = fmt[j];
		if (c == '+')
			f.f_plus = 1;
		else if (c == ' ')
			f.f_space = 1;
		else if (c == '#')
			f.f_hash = 1;
		else if (c == '0')
			f.f_zero = 1;
		else
			f.f_minus = 1;
		j++;
	}

	/* width */
	if (fmt[j] == '*')
	{
		f.width = va_arg(*ap, int);
		j++;
	}
	else if (is_digit((unsigned char)fmt[j]))
	{
		val = 0;
		while (is_digit((unsigned char)fmt[j]))
		{
			val = val * 10 + (fmt[j] - '0');
			j++;
		}
		f.width = val;
	}

	/* precision */
	if (fmt[j] == '.')
	{
		j++;
		f.precision = 0;
		if (fmt[j] == '*')
		{
			f.precision = va_arg(*ap, int);
			j++;
		}
		else
		{
			val = 0;
			while (is_digit((unsigned char)fmt[j]))
			{
				val = val * 10 + (fmt[j] - '0');
				j++;
			}
			f.precision = val;
		}
	}

	/* length */
	if (fmt[j] == 'h')
	{
		f.length = 1;
		j++;
	}
	else if (fmt[j] == 'l')
	{
		f.length = 2;
		j++;
	}

	/* specifier */
	c = fmt[j];
	if (!c)
		return (-1);
	f.spec = (char)c;
	*out = f;
	*i = j + 1; /* next char after spec */
	return (1);
}
