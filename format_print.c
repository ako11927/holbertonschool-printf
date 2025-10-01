#include "main.h"
#include <stdarg.h>
#include <string.h>

/* public: declared in main.h */
int putnchar(char c, int n)
{
	int k = 0;

	while (n-- > 0)
	{
		if (_putchar(c) < 0)
			return (-1);
		k++;
	}
	return (k);
}

/* private helpers */
static char *utoa_base(unsigned long v, char *buf, int base, int upper)
{
	static const char lo[] = "0123456789abcdef";
	static const char up[] = "0123456789ABCDEF";
	const char *digits = upper ? up : lo;
	char *p = buf + 65;

	*p = '\0';
	if (v == 0)
	{
		*--p = '0';
		return (p);
	}
	while (v)
	{
		*--p = digits[v % (unsigned)base];
		v /= (unsigned)base;
	}
	return (p);
}

static int print_str_fmt(const char *s, const fmt_t *f)
{
	int n, pad, count = 0, i, k;

	if (!s)
		s = "(null)";

	if (f->precision >= 0)
	{
		for (i = 0; s[i] && i < f->precision; i++)
			;
		n = i;
	}
	else
		n = (int)strlen(s);

	pad = (f->width > n) ? f->width - n : 0;

	if (!f->f_minus)
	{
		k = putnchar(' ', pad);
		if (k < 0)
			return (-1);
		count += k;
	}
	for (i = 0; i < n; i++)
	{
		if (_putchar(s[i]) < 0)
			return (-1);
		count++;
	}
	if (f->f_minus)
	{
		k = putnchar(' ', pad);
		if (k < 0)
			return (-1);
		count += k;
	}
	return (count);
}

static int print_char_fmt(int c, const fmt_t *f)
{
	int pad = (f->width > 1) ? f->width - 1 : 0, count = 0, k;

	if (!f->f_minus)
	{
		k = putnchar(' ', pad);
		if (k < 0)
			return (-1);
		count += k;
	}
	if (_putchar((char)c) < 0)
		return (-1);
	count++;
	if (f->f_minus)
	{
		k = putnchar(' ', pad);
		if (k < 0)
			return (-1);
		count += k;
	}
	return (count);
}

static int print_intlike(long val, const fmt_t *f, int is_unsigned, int base, int upper)
{
	char buf[70], prefix[3];
	unsigned long u;
	char *digits;
	int nlen, zeroes, pad, prelen = 0, count = 0;
	int use_zero_pad, i, k;

	use_zero_pad = (f->f_zero && !f->f_minus && f->precision < 0);

	if (!is_unsigned && val < 0)
	{
		u = (unsigned long)(-val);
		prefix[prelen++] = '-';
	}
	else
	{
		u = (unsigned long)val;
		if (!is_unsigned)
		{
			if (f->f_plus)
				prefix[prelen++] = '+';
			else if (f->f_space)
				prefix[prelen++] = ' ';
		}
	}

	digits = utoa_base(u, buf, base, upper);
	nlen = (f->precision == 0 && u == 0) ? 0 : (int)strlen(digits);

	if (f->f_hash)
	{
		if (base == 8 && (u != 0 || f->precision == 0))
			prefix[prelen++] = '0';
		else if (base == 16 && u != 0)
		{
			prefix[prelen++] = '0';
			prefix[prelen++] = upper ? 'X' : 'x';
		}
	}

	zeroes = (f->precision > nlen) ? f->precision - nlen : 0;
	pad = f->width - (prelen + zeroes + nlen);
	if (pad < 0)
		pad = 0;

	/* left pad + prefix */
	if (!f->f_minus)
	{
		if (use_zero_pad)
		{
			for (i = 0; i < prelen; i++)
			{
				if (_putchar(prefix[i]) < 0)
					return (-1);
				count++;
			}
			k = putnchar('0', pad);
			if (k < 0)
				return (-1);
			count += k;
		}
		else
		{
			k = putnchar(' ', pad);
			if (k < 0)
				return (-1);
			count += k;

			for (i = 0; i < prelen; i++)
			{
				if (_putchar(prefix[i]) < 0)
					return (-1);
				count++;
			}
		}
	}
	else
	{
		for (i = 0; i < prelen; i++)
		{
			if (_putchar(prefix[i]) < 0)
				return (-1);
			count++;
		}
	}

	/* precision zeros */
	k = putnchar('0', zeroes);
	if (k < 0)
		return (-1);
	count += k;

	/* digits */
	for (i = 0; i < nlen; i++)
	{
		if (_putchar(digits[i]) < 0)
			return (-1);
		count++;
	}

	/* right pad */
	if (f->f_minus)
	{
		k = putnchar(' ', pad);
		if (k < 0)
			return (-1);
		count += k;
	}
	return (count);
}

/**
 * print_formatted - dispatch one parsed conversion and print it
 * @f: parsed format (flags/width/precision/length/spec)
 * @ap: pointer to va_list (as declared in main.h)
 * Return: chars printed or -1
 */
int print_formatted(const fmt_t *f, va_list *ap)
{
	fmt_t tmp;
	int up;

	tmp = *f;

	switch (f->spec)
	{
	case 'c':
		return (print_char_fmt(va_arg(*ap, int), f));
	case '%':
		return (print_char_fmt('%', f));
	case 's':
		return (print_str_fmt(va_arg(*ap, char *), f));
	case 'S':
		return (print_S(va_arg(*ap, char *)));
	case 'r':
		return (print_rev(va_arg(*ap, char *)));
	case 'R':
		return (print_rot13(va_arg(*ap, char *)));
	case 'p':
	{
		void *ptr = va_arg(*ap, void *);
		if (!ptr)
			return (print_str_fmt("(nil)", f));
		tmp.f_hash = 1;
		return (print_intlike((long)(unsigned long)ptr, &tmp, 1, 16, 0));
	}
	case 'b':
		return (print_intlike((long)(unsigned)va_arg(*ap, unsigned int), f, 1, 2, 0));
	case 'd':
	case 'i':
		if (f->length == 'l')
			return (print_intlike(va_arg(*ap, long), f, 0, 10, 0));
		else if (f->length == 'h')
			return (print_intlike((long)(short)va_arg(*ap, int), f, 0, 10, 0));
		return (print_intlike((long)va_arg(*ap, int), f, 0, 10, 0));
	case 'u':
		if (f->length == 'l')
			return (print_intlike((long)va_arg(*ap, unsigned long), f, 1, 10, 0));
		else if (f->length == 'h')
			return (print_intlike((long)(unsigned short)va_arg(*ap, unsigned int), f, 1, 10, 0));
		return (print_intlike((long)va_arg(*ap, unsigned int), f, 1, 10, 0));
	case 'o':
		if (f->length == 'l')
			return (print_intlike((long)va_arg(*ap, unsigned long), f, 1, 8, 0));
		else if (f->length == 'h')
			return (print_intlike((long)(unsigned short)va_arg(*ap, unsigned int), f, 1, 8, 0));
		return (print_intlike((long)va_arg(*ap, unsigned int), f, 1, 8, 0));
	case 'x':
	case 'X':
		up = (f->spec == 'X');
		if (f->length == 'l')
			return (print_intlike((long)va_arg(*ap, unsigned long), f, 1, 16, up));
		else if (f->length == 'h')
			return (print_intlike((long)(unsigned short)va_arg(*ap, unsigned int), f, 1, 16, up));
		return (print_intlike((long)va_arg(*ap, unsigned int), f, 1, 16, up));
	default:
		if (_putchar('%') < 0 || _putchar(f->spec) < 0)
			return (-1);
		return (2);
	}
}
