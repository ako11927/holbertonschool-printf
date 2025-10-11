#include "main.h"

/* small helpers */
static int padn(char ch, int n)
{
	int k, out = 0;

	while (n-- > 0)
	{
		k = _putchar(ch);
		if (k == -1)
			return (-1);
		out += k;
	}
	return (out);
}

static int print_str_prec(const char *s, int prec)
{
	int out = 0, k, n;
	if (!s)
		s = "(null)";
	if (prec >= 0)
	{
		n = (int)strnlen_prec(s, prec);
		while (n--)
		{
			k = _putchar(*s++);
			if (k == -1)
				return (-1);
			out += k;
		}
		return (out);
	}
	/* no precision limit */
	while (*s)
	{
		k = _putchar(*s++);
		if (k == -1)
			return (-1);
		out += k;
	}
	return (out);
}

static int utoa_rev(unsigned long v, int base, int upper, char *buf)
{
	const char *digits_l = "0123456789abcdef";
	const char *digits_u = "0123456789ABCDEF";
	const char *d = upper ? digits_u : digits_l;
	int i = 0;

	if (v == 0)
	{
		buf[i++] = '0';
		buf[i] = '\0';
		return (i);
	}
	while (v)
	{
		buf[i++] = d[v % (unsigned long)base];
		v /= (unsigned long)base;
	}
	buf[i] = '\0';
	return (i);
}

/**
 * print_formatted - print one parsed directive
 * Handles: c s d i u o x X % with flags + space # 0 - , width, precision,
 * and length h / l (for diouxX).
 */
int print_formatted(const fmt_t *f_in, va_list *ap)
{
	fmt_t f = *f_in;
	int out = 0, k, base, upper, neg, signed_conv, pad, prec, padzero;
	unsigned long uval;
	long sval;
	char tmp[64];      /* enough for 64-bit in any base */
	int len, i;
	char sign = 0;
	const char *prefix = "";
	int prefix_len = 0;

	/* normalize negatives for width/precision */
	if (f.width < 0)
	{
		f.f_minus = 1;
		f.width = -f.width;
	}
	if (f.precision < 0)
		f.precision = -1;

	/* strings */
	if (f.spec == 's')
	{
		const char *s = va_arg(*ap, char *);
		int slen = (s ? (int)strnlen_prec(s, (f.precision >= 0) ? f.precision : -1)
			      : (f.precision >= 0 ? (int)strnlen_prec("(null)", f.precision) : 6));
		int left = f.f_minus;
		int wpad = (f.width > slen) ? (f.width - slen) : 0;

		if (!left && wpad)
		{
			k = padn(' ', wpad);
			if (k == -1)
				return (-1);
			out += k;
		}
		k = print_str_prec(s ? s : "(null)", f.precision);
		if (k == -1)
			return (-1);
		out += k;

		if (left && wpad)
		{
			k = padn(' ', wpad);
			if (k == -1)
				return (-1);
			out += k;
		}
		return (out);
	}

	/* char */
	if (f.spec == 'c')
	{
		int ch = va_arg(*ap, int);
		int wpad = (f.width > 1) ? (f.width - 1) : 0;

		if (!f.f_minus && wpad)
		{
			k = padn(' ', wpad);
			if (k == -1)
				return (-1);
			out += k;
		}
		if (_putchar((char)ch) == -1)
			return (-1);
		out++;

		if (f.f_minus && wpad)
		{
			k = padn(' ', wpad);
			if (k == -1)
				return (-1);
			out += k;
		}
		return (out);
	}

	/* percent literal */
	if (f.spec == '%')
	{
		int wpad = (f.width > 1) ? (f.width - 1) : 0;
		char padc = (!f.f_minus && f.f_zero && f.precision < 0) ? '0' : ' ';

		if (!f.f_minus && wpad)
		{
			k = padn(padc, wpad);
			if (k == -1)
				return (-1);
			out += k;
		}
		if (_putchar('%') == -1)
			return (-1);
		out++;

		if (f.f_minus && wpad)
		{
			k = padn(' ', wpad);
			if (k == -1)
				return (-1);
			out += k;
		}
		return (out);
	}

	/* numbers: d i u o x X */
	if (f.spec == 'd' || f.spec == 'i' || f.spec == 'u' ||
	    f.spec == 'o' || f.spec == 'x' || f.spec == 'X')
	{
		signed_conv = (f.spec == 'd' || f.spec == 'i');
		base = (f.spec == 'o') ? 8 : ((f.spec == 'x' || f.spec == 'X') ? 16 : 10);
		upper = (f.spec == 'X');

		/* pull value with length */
		if (signed_conv)
		{
			if (f.length == 2) /* l */
				sval = va_arg(*ap, long);
			else
				sval = (long)va_arg(*ap, int);
			neg = (sval < 0);
			if (neg)
			{
				uval = (unsigned long)(-sval);
				sign = '-';
			}
			else
			{
				uval = (unsigned long)sval;
				if (f.f_plus)
					sign = '+';
				else if (f.f_space)
					sign = ' ';
			}
		}
		else
		{
			if (f.length == 2) /* l */
				uval = va_arg(*ap, unsigned long);
			else
				uval = (unsigned long)va_arg(*ap, unsigned int);
		}

		/* convert */
		len = utoa_rev(uval, base, upper, tmp); /* tmp is reversed */
		/* precision==0 and value==0 prints nothing for numbers */
		if (f.precision == 0 && uval == 0)
			len = 0;

		/* apply '#' */
		if (f.f_hash && uval != 0)
		{
			if (base == 8)
			{
				prefix = "0";
				prefix_len = 1;
			}
			else if (base == 16)
			{
				prefix = upper ? "0X" : "0x";
				prefix_len = 2;
			}
		}

		/* width / precision calcs */
		prec = (f.precision >= 0) ? f.precision : 0;
		if (prec < len)
			prec = len;
		padzero = (f.precision < 0 && f.f_zero && !f.f_minus) ? 1 : 0;

		/* total content width = sign + prefix + max(prec, len) */
		i = prec;
		if (sign)
			i++;
		if (prefix_len)
			i += prefix_len;

		pad = (f.width > i) ? (f.width - i) : 0;

		/* left spaces */
		if (!f.f_minus && !padzero && pad)
		{
			k = padn(' ', pad);
			if (k == -1)
				return (-1);
			out += k;
			pad = 0;
		}

		/* sign */
		if (sign)
		{
			if (_putchar(sign) == -1)
				return (-1);
			out++;
			sign = 0;
		}

		/* prefix */
		if (prefix_len == 2)
		{
			if (_putchar(prefix[0]) == -1)
				return (-1);
			out++;
			if (_putchar(prefix[1]) == -1)
				return (-1);
			out++;
		}
		else if (prefix_len == 1)
		{
			if (_putchar(prefix[0]) == -1)
				return (-1);
			out++;
		}

		/* zero padding (from width via '0' when no precision) */
		if (!f.f_minus && padzero && pad)
		{
			k = padn('0', pad);
			if (k == -1)
				return (-1);
			out += k;
			pad = 0;
		}

		/* precision zeros */
		if (prec > len)
		{
			k = padn('0', prec - len);
			if (k == -1)
				return (-1);
			out += k;
		}

		/* number digits (remember tmp is reversed) */
		for (i = len - 1; i >= 0; i--)
		{
			if (_putchar(tmp[i]) == -1)
				return (-1);
			out++;
		}

		/* right spaces */
		if (f.f_minus && pad)
		{
			k = padn(' ', pad);
			if (k == -1)
				return (-1);
			out += k;
		}
		return (out);
	}

	/* keep your custom ones working (S, r, R, b, p) */
	if (f.spec == 'S')
	{
		const char *sS = va_arg(*ap, char *);
		return (print_S(sS ? sS : "(null)"));
	}
	if (f.spec == 'r')
	{
		const char *sr = va_arg(*ap, char *);
		return (print_rev(sr ? sr : "(null)"));
	}
	if (f.spec == 'R')
	{
		const char *sR = va_arg(*ap, char *);
		return (print_rot13(sR ? sR : "(null)"));
	}

	/* pointer: honor width/precision minimally via string formatting */
	if (f.spec == 'p')
	{
		/* delegate to your pointer printer; width/prec not required by task */
		const void *pv = va_arg(*ap, const void *);
		return (print_pointer(pv));
	}

	/* Unknown spec: print '%' then the char (printf behaviour) */
	if (_putchar('%') == -1)
		return (-1);
	out++;
	if (_putchar(f.spec) == -1)
		return (-1);
	out++;
	return (out);
}
