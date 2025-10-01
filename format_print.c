#include "main.h"

/**
 * utoa_base - convert unsigned int to base string
 * @n: number
 * @base: 2..16
 * @upper: non-zero => uppercase hex
 * @buf: output buffer (>= 33 bytes)
 * Return: length (no '\0')
 */
int utoa_base(unsigned int n, unsigned int base, int upper, char *buf)
{
	const char *digs = upper ?
		"0123456789ABCDEF" : "0123456789abcdef";
	char tmp[33];
	int i = 0, j;

	if (base < 2 || base > 16)
		return (0);
	if (n == 0)
	{
		buf[0] = '0', buf[1] = '\0';
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
 * print_str_fmt - formatted %s with width/precision and '-'
 * @f: parsed fields
 * @ap: va_list*
 * Return: chars printed or -1
 */
int print_str_fmt(const fmt_t *f, va_list *ap)
{
	char *s = va_arg(*ap, char *);
	int slen = 0, use, pad, wrote = 0, k, i;

	if (!s)
		s = "(null)";
	while (s[slen] != '\0')
		slen++;

	use = (f->precision >= 0 && f->precision < slen) ? f->precision : slen;
	pad = (f->width > use) ? (f->width - use) : 0;

	if (!f->f_minus && pad)
	{
		k = putnchar(' ', pad);
		if (k == -1) return (-1);
		wrote += k;
	}
	for (i = 0; i < use; i++)
	{
		if (_putchar(s[i]) == -1) return (-1);
		wrote++;
	}
	if (f->f_minus && pad)
	{
		k = putnchar(' ', pad);
		if (k == -1) return (-1);
		wrote += k;
	}
	return (wrote);
}

/**
 * print_char_fmt - formatted %c with width and '-'
 * @f: parsed fields
 * @ap: va_list*
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
		if (k == -1) return (-1);
		wrote += k;
	}
	if (_putchar(c) == -1) return (-1);
	wrote++;
	if (f->f_minus && pad)
	{
		k = putnchar(' ', pad);
		if (k == -1) return (-1);
		wrote += k;
	}
	return (wrote);
}

/**
 * print_intlike - formatted %d %i %u %o %x %X
 * @f: parsed fields
 * @ap: va_list*
 * Return: chars printed or -1
 */
int print_intlike(const fmt_t *f, va_list *ap)
{
	int is_d = (f->spec == 'd' || f->spec == 'i');
	unsigned int base = (f->spec == 'o') ? 8 :
		((f->spec == 'x' || f->spec == 'X') ? 16 : 10);
	int upper = (f->spec == 'X');
	unsigned int u;
	int v, wrote = 0, k, i;
	char signch = 0, num[33], prefix[3];
	int nlen, prelen = 0, zeros, pad, pad0;

	if (is_d)
	{
		v = va_arg(*ap, int);
		if (v < 0)
		{
			signch = '-', u = (unsigned int)(-v);
		}
		else
		{
			u = (unsigned int)v;
			if (f->f_plus) signch = '+';
			else if (f->f_space) signch = ' ';
		}
	}
	else
		u = va_arg(*ap, unsigned int);

	nlen = utoa_base(u, base, upper, num);

	prefix[0] = '\0', prelen = 0;
	if (f->f_hash && u != 0)
	{
		if (base == 8)
			prefix[0] = '0', prefix[1] = '\0', prelen = 1;
		else if (base == 16)
			prefix[0] = '0', prefix[1] = upper ? 'X' : 'x',
			prefix[2] = '\0', prelen = 2;
	}
	if (signch) prelen += 1;

	if (f->precision >= 0)
	{
		zeros = (f->precision > nlen) ? (f->precision - nlen) : 0;
		pad0 = 0;
	}
	else
	{
		zeros = 0;
		pad0 = (f->f_zero && !f->f_minus);
	}

	pad = f->width - (prelen + zeros + nlen);
	if (pad < 0) pad = 0;

	if (!f->f_minus && pad && !pad0)
	{
		k = putnchar(' ', pad);
		if (k == -1) return (-1);
		wrote += k;
	}
	if (signch)
	{
		if (_putchar(signch) == -1) return (-1);
		wrote++;
	}
	if (prelen == 2)
	{
		if (_putchar(prefix[0]) == -1 || _putchar(prefix[1]) == -1)
			return (-1);
		wrote += 2;
	}
	else if (prelen == 1)
	{
		if (_putchar('0') == -1) return (-1);
		wrote++;
	}
	if (!f->f_minus && pad && pad0)
	{
		k = putnchar('0', pad);
		if (k == -1) return (-1);
		wrote += k;
	}
	if (zeros)
	{
		k = putnchar('0', zeros);
		if (k == -1) return (-1);
		wrote += k;
	}
	for (i = 0; i < nlen; i++)
	{
		if (_putchar(num[i]) == -1) return (-1);
		wrote++;
	}
	if (f->f_minus && pad)
	{
		k = putnchar(' ', pad);
		if (k == -1) return (-1);
		wrote += k;
	}
	return (wrote);
}

/**
 * print_formatted - route by specifier and call proper helper
 * @f: parsed fields
 * @ap: va_list*
 * Return: chars printed or -1
 */
int print_formatted(const fmt_t *f, va_list *ap)
{
	char sp = f->spec;

	if (sp == 's')
		return (print_str_fmt(f, ap));
	if (sp == 'c')
		return (print_char_fmt(f, ap));
	if (sp == 'd' || sp == 'i' || sp == 'u' ||
	    sp == 'o' || sp == 'x' || sp == 'X')
		return (print_intlike(f, ap));

	/* pointer keeps its original helper taking va_list* */
	if (sp == 'p')
		return (print_pointer(ap));

	/* custom helpers in your header take plain args */
	if (sp == 'S')
	{
		char *s = va_arg(*ap, char *);
		return (print_S(s));
	}
	if (sp == 'r')
	{
		char *s = va_arg(*ap, char *);
		return (print_rev(s));
	}
	if (sp == 'R')
	{
		char *s = va_arg(*ap, char *);
		return (print_rot13(s));
	}
	if (sp == 'b')
	{
		unsigned int u = va_arg(*ap, unsigned int);
		return (print_base((unsigned long)u, 2, 0));
	}

	/* unknown spec: print literally */
	if (_putchar('%') == -1 || _putchar(sp) == -1)
		return (-1);
	return (2);
}
