#include "main.h"

/* local helpers */
static int padn(char c, int n)
{
	int i, out = 0;
	for (i = 0; i < n; i++)
	{
		if (_putchar(c) == -1)
			return (-1);
		out++;
	}
	return (out);
}

/* string with width / precision / '-' */
static int print_str_fmt(const fmt_t *f, va_list *ap)
{
	const char *s = va_arg(*ap, char *);
	int n, pad, k, out;

	if (!s) s = "(null)";
	n = (f->precision >= 0) ? (int)strnlen_prec(s, f->precision) : 0;
	if (f->precision < 0)
	{
		const char *t = s;
		n = 0;
		while (*t++) n++;
	}

	pad = (f->width > n) ? f->width - n : 0;
	out = 0;

	if (!f->f_minus && pad)
	{
		k = padn(' ', pad);
		if (k == -1) return (-1);
		out += k;
	}

	k = 0;
	while (k < n)
	{
		if (_putchar(s[k]) == -1) return (-1);
		out++;
		k++;
	}

	if (f->f_minus && pad)
	{
		k = padn(' ', pad);
		if (k == -1) return (-1);
		out += k;
	}
	return (out);
}

/* full integer/uint/o/x/X/b/p engine with flags/width/precision/len */
static int print_intlike(const fmt_t *f_in, va_list *ap)
{
	fmt_t f = *f_in;
	int width, prec, base, uppercase;
	unsigned long uval;
	long sval;
	int neg, is_ptr;
	char tmp[64];
	int nlen, prelen, pad, out, k;
	char signch;
	char prefix[3];
	int use_prefix;

	if (f.width < 0) { f.f_minus = 1; f.width = -f.width; }
	if (f.precision < 0) f.precision = -1;


	is_ptr = 0;
	base = 10;
	uppercase = 0;
	signch = 0;
	use_prefix = 0;
	prefix[0] = prefix[1] = prefix[2] = 0;

	if (f.spec == 'd' || f.spec == 'i')
	{

		if (f.length == 2)
			sval = va_arg(*ap, long);
		else if (f.length == 1)
			sval = (short)va_arg(*ap, int);
		else
			sval = va_arg(*ap, int);

		neg = (sval < 0);
		uval = (unsigned long)(neg ? -sval : sval);
		if (neg) signch = '-';
		else if (f.f_plus) signch = '+';
		else if (f.f_space) signch = ' ';
		base = 10;
	}
	else if (f.spec == 'u' || f.spec == 'o' ||
	         f.spec == 'x' || f.spec == 'X' || f.spec == 'b')
	{

		if (f.length == 2)
			uval = va_arg(*ap, unsigned long);
		else if (f.length == 1)
			uval = (unsigned long)((unsigned short)
				va_arg(*ap, unsigned int));
		else
			uval = (unsigned long)va_arg(*ap, unsigned int);

		if (f.spec == 'o') base = 8;
		else if (f.spec == 'x' || f.spec == 'X') base = 16;
		else if (f.spec == 'b') base = 2;
		if (f.spec == 'X') uppercase = 1;
	}
	else if (f.spec == 'p')
	{

		is_ptr = 1;
		uval = (unsigned long)(unsigned long)va_arg(*ap, void *);
		base = 16;
	}
	else
	{
		return (0);
	}

	nlen = 0;
	if (uval == 0)
	{
		tmp[nlen++] = '0';
	}
	else
	{
		unsigned long v = uval;
		while (v && nlen < (int)(sizeof(tmp)))
		{
			unsigned int d = (unsigned int)(v % (unsigned long)base);
			if (d < 10) tmp[nlen++] = (char)('0' + d);
			else tmp[nlen++] =
				(char)((uppercase ? 'A' : 'a') + (d - 10));
			v /= (unsigned long)base;
		}
	}

	if (f.precision == 0 && uval == 0)
		nlen = 0;

	prelen = (f.precision > nlen) ? f.precision - nlen : 0;

	if (is_ptr)
	{
		prefix[0] = '0'; prefix[1] = 'x';
		use_prefix = 1;
	}
	else if (f.f_hash && uval != 0)
	{
		if (base == 8) { prefix[0] = '0'; use_prefix = 1; }
		else if (base == 16)
		{
			prefix[0] = '0'; prefix[1] = (uppercase ? 'X' : 'x');
			use_prefix = 1;
		}
		else if (base == 2)
		{
			prefix[0] = '0'; prefix[1] = (uppercase ? 'B' : 'b');
			use_prefix = 1;
		}
	}

	width = f.width;
	prec  = f.precision;

	if (f.f_zero && !f.f_minus && prec == -1)
	{
		prec = nlen + prelen;
		if (signch) prec++;
		if (use_prefix) prec += (prefix[1] ? 2 : 1);
		if (prec < width) prec = width;
	}

	pad = 0;
	out = 0;

	if (!f.f_minus)
	{
		int need = 0;
		need = nlen + prelen;
		if (signch) need++;
		if (use_prefix) need += (prefix[1] ? 2 : 1);
		if (width > need) pad = width - need;
		if (pad)
		{
			k = padn(' ', pad);
			if (k == -1) return (-1);
			out += k;
		}
	}

	if (signch)
	{
		if (_putchar(signch) == -1) return (-1);
		out++;
	}

	if (use_prefix)
	{
		if (_putchar(prefix[0]) == -1) return (-1);
		out++;
		if (prefix[1])
		{
			if (_putchar(prefix[1]) == -1) return (-1);
			out++;
		}
	}

	if (prec > nlen)
	{
		k = padn('0', prec - nlen);
		if (k == -1) return (-1);
		out += k;
	}

	while (nlen > 0)
	{
		if (_putchar(tmp[--nlen]) == -1) return (-1);
		out++;
	}

	if (f.f_minus && width > out)
	{
		int used = out;
		while (used < width)
		{
			if (_putchar(' ') == -1) return (-1);
			out++;
			used++;
		}
	}

	return (out);
}

/*
 * print_formatted - central dispatcher for one parsed fmt_t
 * Returns printed chars or -1 on write error.
 */
int print_formatted(const fmt_t *f_in, va_list *ap)
{
	fmt_t f = *f_in;
	int pad, k, out, ch;

	if (f.width < 0) { f.f_minus = 1; f.width = -f.width; }
	if (f.precision < 0) f.precision = -1;

	if (f.spec == 's')
		return (print_str_fmt(&f, ap));

	if (f.spec == 'c')
	{
		out = 0;
		pad = (f.width > 1) ? f.width - 1 : 0;
		if (!f.f_minus && pad)
		{
			k = padn(' ', pad);
			if (k == -1) return (-1);
			out += k;
		}
		ch = va_arg(*ap, int);
		if (_putchar((char)ch) == -1) return (-1);
		out++;
		if (f.f_minus && pad)
		{
			k = padn(' ', pad);
			if (k == -1) return (-1);
			out += k;
		}
		return (out);
	}

	if (f.spec == '%')
	{
		out = 0;
		pad = (f.width > 1) ? f.width - 1 : 0;
		if (!f.f_minus && pad)
		{
			char pc = (f.f_zero ? '0' : ' ');
			k = padn(pc, pad);
			if (k == -1) return (-1);
			out += k;
		}
		if (_putchar('%') == -1) return (-1);
		out++;
		if (f.f_minus && pad)
		{
			k = padn(' ', pad);
			if (k == -1) return (-1);
			out += k;
		}
		return (out);
	}

	if (f.spec == 'd' || f.spec == 'i' || f.spec == 'u' ||
	    f.spec == 'o' || f.spec == 'x' || f.spec == 'X' ||
	    f.spec == 'b' || f.spec == 'p')
		return (print_intlike(&f, ap));

	if (f.spec == 'S')
	{
		const char *s = va_arg(*ap, char *);
		return (print_S(s ? s : "(null)"));
	}
	if (f.spec == 'r')
	{
		const char *s2 = va_arg(*ap, char *);
		return (print_rev(s2 ? s2 : "(null)"));
	}
	if (f.spec == 'R')
	{
		const char *s3 = va_arg(*ap, char *);
		return (print_rot13(s3 ? s3 : "(null)"));
	}

	if (_putchar(f.spec) == -1) return (-1);
	return (1);
}
