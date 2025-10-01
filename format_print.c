#include "main.h"

/* repeat char */
int putnchar(char c, int n)
{
	int k, r;

	for (k = 0; k < n; k++)
	{
		r = _putchar(c);
		if (r < 0)
			return (-1);
	}
	return (n);
}

/* utoa base 2..16 */
static int utoa_base(unsigned long n, int base, int upper, char *buf)
{
	const char *lo = "0123456789abcdef";
	const char *up = "0123456789ABCDEF";
	char tmp[65];
	int len = 0;
	int i;

	if (n == 0)
	{
		buf[0] = '0';
		buf[1] = '\0';
		return (1);
	}
	while (n)
	{
		int v = (int)(n % (unsigned long)base);

		tmp[len++] = upper ? up[v] : lo[v];
		n /= (unsigned long)base;
	}
	for (i = 0; i < len; i++)
		buf[i] = tmp[len - 1 - i];
	buf[len] = '\0';
	return (len);
}

/* ints: d i u o x X */
static int print_intlike(const fmt_t *f, va_list *ap)
{
	int base = 10, upper = 0, is_signed = 0;
	unsigned long u = 0;
	long s = 0;
	int neg = 0, nlen, zeros = 0, pad = 0, left = f->f_minus;
	int total = 0, use_zero_pad;
	char num[65];
	int signlen, altlen, need, r, k;

	if (f->spec == 'd' || f->spec == 'i')
	{
		is_signed = 1;
		base = 10;
	}
	else if (f->spec == 'u')
		base = 10;
	else if (f->spec == 'o')
		base = 8;
	else if (f->spec == 'x' || f->spec == 'X')
	{
		base = 16;
		upper = (f->spec == 'X');
	}
	else
		return (-2);

	/* fetch with l/h */
	if (is_signed)
	{
		if (f->length == 'l')
			s = va_arg(*ap, long);
		else if (f->length == 'h')
			s = (short)va_arg(*ap, int);
		else
			s = va_arg(*ap, int);
		if (s < 0)
		{
			neg = 1;
			u = (unsigned long)(-s);
		}
		else
			u = (unsigned long)s;
	}
	else
	{
		if (f->length == 'l')
			u = va_arg(*ap, unsigned long);
		else if (f->length == 'h')
			u = (unsigned short)va_arg(*ap, unsigned int);
		else
			u = va_arg(*ap, unsigned int);
	}

	nlen = utoa_base(u, base, upper, num);

	/* precision==0 and value==0 => no digits */
	if (f->precision == 0 && u == 0)
		nlen = 0;

	if (f->precision > nlen)
		zeros = f->precision - nlen;

	use_zero_pad = (f->f_zero && !left && f->precision < 0);

	/* compute padding/sign/prefix */
	signlen = (is_signed && (neg || f->f_plus || f->f_space)) ? 1 : 0;
	altlen = 0;
	if (f->f_hash && u != 0 && (f->spec == 'x' || f->spec == 'X'))
		altlen = 2;
	if (f->f_hash && f->spec == 'o' && (u != 0 || f->precision == 0))
	{
		if (!(f->precision > nlen) && !(nlen > 0 && num[0] == '0'))
			altlen = 1;
	}
	need = signlen + altlen + zeros + nlen;
	if (f->width > need)
		pad = f->width - need;

	if (!left && !use_zero_pad && pad)
	{
		r = putnchar(' ', pad);
		if (r < 0)
			return (-1);
		total += r;
		pad = 0;
	}

	/* sign */
	if (is_signed)
	{
		if (neg && _putchar('-') < 0)
			return (-1);
		if (!neg && f->f_plus && _putchar('+') < 0)
			return (-1);
		if (!neg && !f->f_plus && f->f_space && _putchar(' ') < 0)
			return (-1);
		if (neg || f->f_plus || f->f_space)
			total++;
	}

	/* hex 0x/0X */
	if (f->f_hash && u != 0 && (f->spec == 'x' || f->spec == 'X'))
	{
		if (_putchar('0') < 0)
			return (-1);
		if (_putchar(f->spec == 'x' ? 'x' : 'X') < 0)
			return (-1);
		total += 2;
	}

	/* octal leading 0 */
	if (f->f_hash && f->spec == 'o' && (u != 0 || f->precision == 0))
	{
		if (!(f->precision > nlen) && !(nlen > 0 && num[0] == '0'))
		{
			if (_putchar('0') < 0)
				return (-1);
			total += 1;
		}
	}

	/* zero pad from width (after sign/prefix) */
	if (use_zero_pad && pad)
	{
		r = putnchar('0', pad);
		if (r < 0)
			return (-1);
		total += r;
		pad = 0;
	}

	/* zeros from precision */
	if (zeros)
	{
		r = putnchar('0', zeros);
		if (r < 0)
			return (-1);
		total += r;
	}

	/* digits */
	for (k = 0; k < nlen; k++)
	{
		if (_putchar(num[k]) < 0)
			return (-1);
		total++;
	}

	/* left-justify spaces */
	if (left && pad)
	{
		r = putnchar(' ', pad);
		if (r < 0)
			return (-1);
		total += r;
	}
	return (total);
}

/* strings/chars/% with width + precision(for strings) */
static int print_strlike(const fmt_t *f, va_list *ap)
{
	int total = 0, pad, len, outlen, left = f->f_minus;
	const char *s;
	char c;
	int i, r;

	if (f->spec == 's')
	{
		s = va_arg(*ap, const char *);
		if (s == NULL)
			s = "(null)";
		len = 0;
		while (s[len] != '\0')
			len++;
		outlen = (f->precision >= 0 && f->precision < len) ? f->precision : len;
		pad = (f->width > outlen) ? (f->width - outlen) : 0;

		if (!left && pad)
		{
			r = putnchar(' ', pad);
			if (r < 0)
				return (-1);
			total += r;
		}
		for (i = 0; i < outlen; i++)
		{
			if (_putchar(s[i]) < 0)
				return (-1);
			total++;
		}
		if (left && pad)
		{
			r = putnchar(' ', pad);
			if (r < 0)
				return (-1);
			total += r;
		}
		return (total);
	}
	else if (f->spec == 'c' || f->spec == '%')
	{
		c = (f->spec == '%') ? '%' : (char)va_arg(*ap, int);
		pad = (f->width > 1) ? (f->width - 1) : 0;

		if (!left && pad)
		{
			r = putnchar(' ', pad);
			if (r < 0)
				return (-1);
			total += r;
		}
		if (_putchar(c) < 0)
			return (-1);
		total++;
		if (left && pad)
		{
			r = putnchar(' ', pad);
			if (r < 0)
				return (-1);
			total += r;
		}
		return (total);
	}
	return (-2);
}

/* route one specifier */
int print_formatted(const fmt_t *f, va_list *ap)
{
	int r;

	r = print_intlike(f, ap);
	if (r != -2)
		return (r);

	r = print_strlike(f, ap);
	if (r != -2)
		return (r);

	/* custom ones you already support */
	if (f->spec == 'b')
		return (print_base((unsigned long)va_arg(*ap, unsigned int), 2, 0));
	if (f->spec == 'p')
		return (print_pointer(va_arg(*ap, void *)));
	if (f->spec == 'S')
		return (print_S(va_arg(*ap, const char *)));
	if (f->spec == 'r')
		return (print_rev(va_arg(*ap, const char *)));
	if (f->spec == 'R')
		return (print_rot13(va_arg(*ap, const char *)));

	/* unknown: print literally */
	if (_putchar('%') < 0)
		return (-1);
	if (_putchar(f->spec) < 0)
		return (-1);
	return (2);
}
