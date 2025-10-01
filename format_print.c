#include "main.h"

/* ---------- helpers (static) ---------- */

static int padn(char c, int n) { return (n > 0 ? putnchar(c, n) : 0); }

/* convert unsigned long to base string, returns length (no '\0') */
static int utoa_base(unsigned long v, int base, int upper, char *out)
{
	char tmp[64];
	const char *digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
	int n = 0, i;

	if (base < 2 || base > 16 || out == (char *)0)
		return (-1);
	if (v == 0)
	{
		out[0] = '0';
		return (1);
	}
	while (v)
	{
		tmp[n++] = digits[v % (unsigned long)base];
		v /= (unsigned long)base;
	}
	for (i = 0; i < n; i++)
		out[i] = tmp[n - 1 - i];
	return (n);
}

/* ---------- string formatter ---------- */

int print_str_fmt(const fmt_t *f, va_list *ap)
{
	char *s = va_arg(*ap, char *);
	int want = f->precision;     /* -1 => unlimited */
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

/* ---------- integer/unsigned/o/x/X/b formatter ---------- */

int print_intlike(const fmt_t *f_in, va_list *ap)
{
	fmt_t f = *f_in; /* work on a copy so we can normalize */
	unsigned long uval = 0;
	int is_signed = 0, base = 10, upper = 0, is_ptr = 0;
	int prelen = 0, nd = 0, k, out = 0;
	char sign = 0, prefix[2] = {0, 0}, digits[64];
	int prec_zeros = 0, left_spaces = 0, zero_pad = 0, right_spaces = 0;
	int width, prec;

	/* width/precision from '*' possibly normalized by parser; keep negatives */
	if (f.width < 0)
	{
		f.f_minus = 1;
		f.width = -f.width;
	}
	if (f.precision < 0)
		f.precision = -1;

	width = f.width;
	prec = f.precision;

	/* type & value */
	switch (f.spec)
	{
	case 'd': case 'i':
		is_signed = 1;
		if (f.length == 2)      /* 'l' */
		{
			long v = va_arg(*ap, long);
			if (v < 0) { sign = '-'; uval = (unsigned long)(-v); }
			else { if (f.f_plus) sign = '+'; else if (f.f_space) sign = ' '; uval = (unsigned long)v; }
		}
		else if (f.length == 1) /* 'h' via default argument promotion */
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

	case 'u': base = 10; uval = (f.length == 2) ? va_arg(*ap, unsigned long)
				: (unsigned long)va_arg(*ap, unsigned int); break;

	case 'o': base = 8;  uval = (f.length == 2) ? va_arg(*ap, unsigned long)
				: (unsigned long)va_arg(*ap, unsigned int); break;

	case 'x': base = 16; upper = 0; uval = (f.length == 2) ? va_arg(*ap, unsigned long)
				: (unsigned long)va_arg(*ap, unsigned int); break;

	case 'X': base = 16; upper = 1; uval = (f.length == 2) ? va_arg(*ap, unsigned long)
				: (unsigned long)va_arg(*ap, unsigned int); break;

	case 'b': base = 2;  uval = (unsigned long)va_arg(*ap, unsigned int); break;

	case 'p': base = 16; upper = 0; is_ptr = 1; uval = (unsigned long)va_arg(*ap, void *); break;

	default: return (-1);
	}

	/* number to string */
	nd = utoa_base(uval, base, upper, digits);
	if (nd == -1)
		return (-1);

	/* precision==0 and value==0 => print nothing (but keep octal '#' as a single '0') */
	if (prec == 0 && uval == 0)
	{
		if (!(f.spec == 'o' && f.f_hash))
			nd = 0;
	}

	/* prefix: '#', pointer, sign */
	if (is_ptr || ((f.f_hash && uval != 0) && (f.spec == 'x' || f.spec == 'X')))
	{
		prefix[0] = '0';
		prefix[1] = (upper ? 'X' : 'x');
		prelen = 2;
	}
	else if (f.f_hash && f.spec == 'o' && (uval != 0 || prec == 0))
	{
		prefix[0] = '0';
		prelen = 1;
	}

	if (sign)
		prelen += 0; /* sign is printed separately but counted in width calc */

	/* zero/space padding calculation */
	if (prec >= 0)
	{
		/* precision specified => '0' flag ignored */
		prec_zeros = (prec > nd) ? (prec - nd) : 0;
		left_spaces = width - ( (sign ? 1 : 0) + prelen + (nd + prec_zeros) );
		if (left_spaces < 0) left_spaces = 0;
	}
	else
	{
		prec_zeros = 0;
		if (f.f_zero && !f.f_minus)
		{
			zero_pad = width - ((sign ? 1 : 0) + prelen + nd);
			if (zero_pad < 0) zero_pad = 0;
		}
		else
		{
			left_spaces = width - ((sign ? 1 : 0) + prelen + nd);
			if (left_spaces < 0) left_spaces = 0;
		}
	}

	/* left spaces */
	if (!f.f_minus && left_spaces)
	{
		k = padn(' ', left_spaces);
		if (k == -1) return (-1);
		out += k;
	}

	/* sign */
	if (sign)
	{
		if (_putchar(sign) == -1) return (-1);
		out++;
	}

	/* 0x / 0 / 0X */
	if (prelen == 2)
	{
		if (_putchar(prefix[0]) == -1 || _putchar(prefix[1]) == -1)
			return (-1);
		out += 2;
	}
	else if (prelen == 1)
	{
		if (_putchar('0') == -1) return (-1);
		out++;
	}

	/* zero padding coming from '0' flag */
	if (zero_pad)
	{
		k = padn('0', zero_pad);
		if (k == -1) return (-1);
		out += k;
	}

	/* precision zeros */
	if (prec_zeros)
	{
		k = padn('0', prec_zeros);
		if (k == -1) return (-1);
		out += k;
	}

	/* number digits (may be zero length when prec==0 && value==0) */
	{
		int i;
		for (i = 0; i < nd; i++)
		{
			if (_putchar(digits[i]) == -1)
				return (-1);
			out++;
		}
	}

	/* right spaces for '-' */
	if (f.f_minus)
	{
		right_spaces = width - out;
		if (right_spaces > 0)
		{
			k = padn(' ', right_spaces);
			if (k == -1) return (-1);
			out += k;
		}
	}

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

		if (!f.f_minus && pad)
		{
			k = padn(' ', pad);
			if (k == -1) return (-1);
			out += k;
		}
		if (_putchar(ch) == -1)
			return (-1);
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
		char *s = va_arg(*ap, char *);
		return (print_S(s ? s : "(null)"));
	}
	if (f.spec == 'r')
	{
		char *s = va_arg(*ap, char *);
		return (print_rev(s ? s : "(null)"));
	}
	if (f.spec == 'R')
	{
		char *s = va_arg(*ap, char *);
		return (print_rot13(s ? s : "(null)"));
	}

	/* unknown: print literally "%<spec>" */
	if (_putchar('%') == -1 || _putchar(f.spec) == -1)
		return (-1);
	return (2);
}
