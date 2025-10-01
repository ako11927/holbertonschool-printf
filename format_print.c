#include "main.h"

/* local helpers must be C90-friendly (no mixed decls, no for-init decls) */

/**
 * utoa_base - convert unsigned int to string in given base
 * @n: number
 * @base: 2..16
 * @upper: non-zero -> uppercase hex
 * @buf: output buffer (at least 33 bytes)
 *
 * Return: length written (not counting '\0')
 */
int utoa_base(unsigned int n, unsigned int base, int upper, char *buf)
{
	char tmp[33];
	const char *digs = upper ? "0123456789ABCDEF" : "0123456789abcdef";
	int i = 0, j;

	if (base < 2 || base > 16)
		return (0);

	if (n == 0)
	{
		buf[0] = '0';
		buf[1] = '\0';
		return (1);
	}

	while (n)
	{
		tmp[i++] = digs[n % base];
		n /= base;
	}
	for (j = 0; j < i; j++)
		buf[j] = tmp[i - 1 - j];
	buf[i] = '\0';
	return (i);
}

/**
 * print_str_fmt - formatted %s
 * @f: parsed format
 * @ap: pointer to va_list
 * Return: chars printed or -1
 */
int print_str_fmt(const fmt_t *f, va_list *ap)
{
	char *s = va_arg(*ap, char *);
	int slen = 0, use = 0, pad = 0, wrote = 0, k, i;

	if (!s)
		s = "(null)";
	while (s[slen] != '\0')
		slen++;

	use = (f->precision >= 0 && f->precision < slen) ? f->precision : slen;
	pad = (f->width > use) ? (f->width - use) : 0;

	if (!f->f_minus && pad)
	{
		k = putnchar(' ', pad);
		if (k == -1)
			return (-1);
		wrote += k;
	}
	for (i = 0; i < use; i++)
	{
		if (_putchar(s[i]) == -1)
			return (-1);
		wrote++;
	}
	if (f->f_minus && pad)
	{
		k = putnchar(' ', pad);
		if (k == -1)
			return (-1);
		wrote += k;
	}
	return (wrote);
}

/**
 * print_char_fmt - formatted %c
 * @f: parsed format
 * @ap: pointer to va_list
 * Return: chars printed or -1
 */
int print_char_fmt(const fmt_t *f, va_list *ap)
{
	char c = (char)va_arg(*ap, int);
	int pad = (f->width > 1) ? (f->width - 1) : 0;
	int wrote = 0, k;

	if (!f->f_minus && pad)
	{
		k = putnchar(' ', pad);
		if (k == -1)
			return (-1);
		wrote += k;
	}
	if (_putchar(c) == -1)
		return (-1);
	wrote++;

	if (f->f_minus && pad)
	{
		k = putnchar(' ', pad);
		if (k == -1)
			return (-1);
		wrote += k;
	}
	return (wrote);
}

/**
 * print_intlike - formatted %d %i %u %o %x %X
 * @f: parsed format
 * @ap: pointer to va_list
 * Return: chars printed or -1
 */
int print_intlike(const fmt_t *f, va_list *ap)
{
	int is_signed = (f->spec == 'd' || f->spec == 'i');
	unsigned int base = 10;
	int upper = 0;
	int wrote = 0, k, i;
	char sign = 0, prefix[3];
	char num[33];
	int nlen, prelen = 0, zeros = 0, pad, pad_with_zero;

	unsigned int u;
	int v;

	if (f->spec == 'o')
		base = 8;
	else if (f->spec == 'x' || f->spec == 'X')
	{
		base = 16;
		upper = (f->spec == 'X');
	}

	if (is_signed)
	{
		v = va_arg(*ap, int);
		if (v < 0)
		{
			sign = '-';
			u = (unsigned int)(-v);
		}
		else
		{
			u = (unsigned int)v;
			if (f->f_plus)
				sign = '+';
			else if (f->f_space)
				sign = ' ';
		}
	}
	else
	{
		u = va_arg(*ap, unsigned int);
	}

	nlen = utoa_base(u, base, upper, num);

	/* prefix for # flag */
	prefix[0] = '\0';
	if (f->f_hash && u != 0)
	{
		if (base == 8)
		{
			prefix[0] = '0';
			prefix[1] = '\0';
			prelen = 1;
		}
		else if (base == 16)
		{
			prefix[0] = '0';
			prefix[1] = upper ? 'X' : 'x';
			prefix[2] = '\0';
			prelen = 2;
		}
	}

	if (sign)
		prelen += 1;

	/* precision overrides '0' flag */
	if (f->precision >= 0)
	{
		zeros = (f->precision > nlen) ? (f->precision - nlen) : 0;
		pad_with_zero = 0;
	}
	else
	{
		zeros = 0;
		pad_with_zero = f->f_zero && !f->f_minus;
	}

	pad = f->width - (prelen + zeros + nlen);
	if (pad < 0)
		pad = 0;

	/* left pad */
	if (!f->f_minus && pad && !pad_with_zero)
	{
		k = putnchar(' ', pad);
		if (k == -1)
			return (-1);
		wrote += k;
	}

	/* sign and/or prefix */
	if (sign)
	{
		if (_putchar(sign) == -1)
			return (-1);
		wrote++;
	}
	if (prelen && prefix[0] == '0' && (prefix[1] == 'x' || prefix[1] == 'X'))
	{
		if (_putchar(prefix[0]) == -1 || _putchar(prefix[1]) == -1)
			return (-1);
		wrote += 2;
	}
	else if (prelen == 1 && prefix[0] == '0')
	{
		if (_putchar('0') == -1)
			return (-1);
		wrote++;
	}

	/* if pad with zeros (width), do it here */
	if (!f->f_minus && pad && pad_with_zero)
	{
		k = putnchar('0', pad);
		if (k == -1)
			return (-1);
		wrote += k;
	}

	/* precision zeros */
	if (zeros)
	{
		k = putnchar('0', zeros);
		if (k == -1)
			return (-1);
		wrote += k;
	}

	/* number itself */
	for (i = 0; i < nlen; i++)
	{
		if (_putchar(num[i]) == -1)
			return (-1);
		wrote++;
	}

	/* right pad */
	if (f->f_minus && pad)
	{
		k = putnchar(' ', pad);
		if (k == -1)
			return (-1);
		wrote += k;
	}

	return (wrote);
}

/**
 * print_formatted - route to a formatter based on spec
 * @f: parsed format
 * @ap: pointer to va_list
 * Return: chars printed or -1
 */
int print_formatted(const fmt_t *f, va_list *ap)
{
	char sp = f->spec;

	if (sp == 's')
		return (print_str_fmt(f, ap));
	if (sp == 'c')
		return (print_char_fmt(f, ap));
	if (sp == 'd' || sp == 'i' || sp == 'u' || sp == 'o' || sp == 'x' || sp == 'X')
		return (print_intlike(f, ap));

	/* Defer custom/legacy ones to existing helpers */
	if (sp == 'p')
		return (print_pointer(ap));
	if (sp == 'S')
		return (print_S(ap));
	if (sp == 'r')
		return (print_rev(ap));
	if (sp == 'R')
		return (print_rot13(ap));
	if (sp == 'b') /* binary, if you kept a helper */
		return (print_base(ap));

	/* unknown spec: print literally like %<char> */
	if (_putchar('%') == -1 || _putchar(sp) == -1)
		return (-1);
	return (2);
}
