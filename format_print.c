#include "main.h"

/* ---------- helpers (static) ---------- */

static int padn(char c, int n)
{
	int out = 0;
	while (n-- > 0)
	{
		if (_putchar(c) == -1)
			return (-1);
		out++;
	}
	return (out);
}

/* ---------- string formatter ---------- */

int print_str_fmt(const fmt_t *f, va_list *ap)
{
	char *s = va_arg(*ap, char *);
	int want = f->precision; /* -1 => unlimited */
	int i, slen = 0, pad = 0, out = 0, k;

	if (s == (char *)0)
		s = "(null)";

	for (i = 0; s[i] && (want < 0 || i < want); i++)
		; /* measure with precision cap */
	slen = i;

	if (f->width > slen)
		pad = f->width - slen;

	if (!f->f_minus && pad)
	{
		k = padn(' ', pad);
		if (k == -1)
			return (-1);
		out += k;
	}
	for (i = 0; i < slen; i++)
	{
		if (_putchar(s[i]) == -1)
			return (-1);
		out++;
	}
	if (f->f_minus && pad)
	{
		k = padn(' ', pad);
		if (k == -1)
			return (-1);
		out += k;
	}
	return (out);
}

/* -------- integer / unsigned / o / x / X / b / p formatter -------- */

int print_intlike(const fmt_t *f_in, va_list *ap)
{
	fmt_t f = *f_in; /* normalize a working copy */
	unsigned long uval = 0;
	int base = 10, upper = 0, is_ptr = 0;
	int prelen = 0, nd = 0, k = 0, out = 0;
	char sign = 0, prefix[2] = {0, 0}, digits[64];
	int prec_zeros = 0, left_spaces = 0, zero_pad = 0, right_spaces = 0;
	int width, prec;

	/* normalize negative width/precision from '*' */
	if (f.width < 0) { f.f_minus = 1; f.width = -f.width; }
	if (f.precision < 0) f.precision = -1;

	/* select type/value */
	switch (f.spec)
	{
	case 'd': case 'i':
		if (f.length == 2) /* l */
		{
			long v = va_arg(*ap, long);
			if (v < 0) { sign = '-'; uval = (unsigned long)(-v); }
			else { if (f.f_plus) sign = '+'; else if (f.f_space) sign = ' '; uval = (unsigned long)v; }
		}
		else if (f.length == 1) /* h */
		{
			int v = va_arg(*ap, int);
			short sv = (short)v;
			if (sv < 0) { sign = '-'; uval = (unsigned long)(-sv); }
			else { if (f.f_plus) sign = '+'; else if (f.f_space) sign = ' '; uval = (unsigned long)sv; }
		}
		else
		{
			int v = va_arg(*ap, int);
			if (v < 0) { sign = '-'; uval = (unsigned long)(-v); }
			else { if (f.f_plus) sign = '+'; else if (f.f_space) sign = ' '; uval = (unsigned long)v; }
		}
		base = 10;
		break;

	case 'u':
		if (f.length == 2) uval = va_arg(*ap, unsigned long);
		else if (f.length == 1) { unsigned int t = va_arg(*ap, unsigned int); uval = (unsigned long)((unsigned short)t); }
		else uval = (unsigned long)va_arg(*ap, unsigned int);
		base = 10;
		break;

	case 'o':
		if (f.length == 2) uval = va_arg(*ap, unsigned long);
		else if (f.length == 1) { unsigned int t = va_arg(*ap, unsigned int); uval = (unsigned long)((unsigned short)t); }
		else uval = (unsigned long)va_arg(*ap, unsigned int);
		base = 8;
		break;

	case 'x':
		upper = 0;
		if (f.length == 2) uval = va_arg(*ap, unsigned long);
		else if (f.length == 1) { unsigned int t = va_arg(*ap, unsigned int); uval = (unsigned long)((unsigned short)t); }
		else uval = (unsigned long)va_arg(*ap, unsigned int);
		base = 16;
		break;

	case 'X':
		upper = 1;
		if (f.length == 2) uval = va_arg(*ap, unsigned long);
		else if (f.length == 1) { unsigned int t = va_arg(*ap, unsigned int); uval = (unsigned long)((unsigned short)t); }
		else uval = (unsigned long)va_arg(*ap, unsigned int);
		base = 16;
		break;

	case 'b':
		base = 2;
		uval = (unsigned long)va_arg(*ap, unsigned int);
		break;

	case 'p':
		base = 16; upper = 0; is_ptr = 1;
		uval = (unsigned long)va_arg(*ap, void *);
		break;

	default:
		return (-1);
	}

	/* prefixes */
	if (is_ptr)
	{
		prefix[0] = '0'; prefix[1] = 'x'; prelen = 2;
	}
	else if (f.f_hash && uval != 0)
	{
		if (base == 8) { prefix[0] = '0'; prelen = 1; }
		else if (base == 16) { prefix[0] = '0'; prefix[1] = (upper ? 'X' : 'x'); prelen = 2; }
	}

	/* convert to reversed digits */
	if (uval == 0)
		digits[nd++] = '0';
	else
	{
		const char *alpha = upper ? "0123456789ABCDEF" : "0123456789abcdef";
		while (uval)
		{
			digits[nd++] = alpha[uval % (unsigned)base];
			uval /= (unsigned)base;
		}
	}

	/* precision & width */
	width = f.width;
	prec  = f.precision;

	if (prec != -1 && prec > nd)
		prec_zeros = prec - nd;

	/* zero value with .0 => print nothing, EXCEPT %#.0o must print one '0' */
	if (prec == 0 && nd == 1 && digits[0] == '0')
	{
		if (base == 8 && f.f_hash)
		{
			/* keep single '0' (nd stays 1) */
		}
		else
		{
			nd = 0;
		}
	}

	/* padding decision */
	if (!f.f_minus)
	{
		int core = nd + prec_zeros + prelen + (sign ? 1 : 0);
		if (f.f_zero && prec == -1)
		{
			zero_pad = width - core;
			if (zero_pad < 0) zero_pad = 0;
		}
		else
		{
			left_spaces = width - core;
			if (left_spaces < 0) left_spaces = 0;
		}
	}
	else
	{
		right_spaces = width - (nd + prec_zeros + prelen + (sign ? 1 : 0));
		if (right_spaces < 0) right_spaces = 0;
	}

	/* emit, in order */
	if (left_spaces) { k = padn(' ', left_spaces); if (k == -1) return (-1); out += k; }
	if (sign)        { if (_putchar(sign) == -1) return (-1); out++; }
	if (prelen >= 1) { if (_putchar(prefix[0]) == -1) return (-1); out++; }
	if (prelen == 2) { if (_putchar(prefix[1]) == -1) return (-1); out++; }
	if (zero_pad)    { k = padn('0', zero_pad); if (k == -1) return (-1); out += k; }
	if (prec_zeros)  { k = padn('0', prec_zeros); if (k == -1) return (-1); out += k; }
	while (nd > 0)   { if (_putchar(digits[--nd]) == -1) return (-1); out++; }
	if (right_spaces){ k = padn(' ', right_spaces); if (k == -1) return (-1); out += k; }

	return (out);
}

/* ---------- master dispatcher ---------- */

int print_formatted(const fmt_t *f_in, va_list *ap)
{
	fmt_t f = *f_in;

	/* normalize negative width/precision from '*' here too */
	if (f.width < 0) { f.f_minus = 1; f.width = -f.width; }
	if (f.precision < 0) f.precision = -1;

	if (f.spec == 's')
		return (print_str_fmt(&f, ap));

	if (f.spec == 'c')
	{
		char ch = (char)va_arg(*ap, int);
		int pad = (f.width > 1) ? f.width - 1 : 0, k, out = 0;

		if (!f.f_minus && pad) { k = padn(' ', pad); if (k == -1) return (-1); out += k; }
		if (_putchar(ch) == -1) return (-1);
		out++;
		if (f.f_minus && pad) { k = padn(' ', pad); if (k == -1) return (-1); out += k; }
		return (out);
	}

	if (f.spec == '%')
	{
		int pad = (f.width > 1) ? f.width - 1 : 0, k, out = 0;
		char padc = (!f.f_minus && f.f_zero) ? '0' : ' ';
		if (!f.f_minus && pad) { k = padn(padc, pad); if (k == -1) return (-1); out += k; }
		if (_putchar('%') == -1) return (-1);
		out++;
		if (f.f_minus && pad) { k = padn(' ', pad); if (k == -1) return (-1); out += k; }
		return (out);
	}

	if (f.spec=='d'||f.spec=='i'||f.spec=='u'||f.spec=='o'||f.spec=='x'||f.spec=='X'||f.spec=='b'||f.spec=='p')
		return (print_intlike(&f, ap));

	if (f.spec == 'S') { char *s = va_arg(*ap, char *); return (print_S(s ? s : "(null)")); }
	if (f.spec == 'r') { char *s = va_arg(*ap, char *); return (print_rev(s ? s : "(null)")); }
	if (f.spec == 'R') { char *s = va_arg(*ap, char *); return (print_rot13(s ? s : "(null)")); }

	/* unknown: print literally "%<spec>" */
	if (_putchar('%') == -1) return (-1);
	if (_putchar(f.spec) == -1) return (-1);
	return (2);
}
