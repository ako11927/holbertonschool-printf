#include "main.h"

/* space/zero padding helper */
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

/* convert unsigned long to base string (2..16), write forward into out, return len */
static int utoa_base(unsigned long v, int base, int upper, char *out)
{
	const char *digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
	char tmp[65];
	int i, j, k;

	if (base < 2 || base > 16)
		return (0);

	i = 0;
	if (v == 0)
		tmp[i++] = '0';
	else
	{
		while (v)
		{
			tmp[i++] = digits[v % (unsigned)base];
			v /= (unsigned)base;
		}
	}
	for (j = i - 1, k = 0; j >= 0; j--, k++)
		out[k] = tmp[j];

	return (i);
}

/* print one parsed conversion (fmt_t is declared in main.h) */
int print_formatted(const fmt_t *f_in, va_list *ap)
{
	fmt_t f = *f_in;
	int out = 0;

	/* normalize negative width => '-' flag */
	if (f.width < 0)
	{
		f.f_minus = 1;
		f.width = -f.width;
	}

	/* ---------- %s ---------- */
	if (f.spec == 's')
	{
		const char *s = va_arg(*ap, const char *);
		int slen, pad, k;

		if (!s)
			s = "(null)";

		if (f.precision >= 0)
			slen = (int)strnlen_prec(s, f.precision);
		else
			slen = (int)strnlen_prec(s, -1);

		pad = (f.width > slen) ? f.width - slen : 0;

		if (!f.f_minus && pad)
		{
			k = padn(' ', pad);
			if (k == -1)
				return (-1);
			out += k;
		}
		for (k = 0; k < slen; k++)
		{
			if (_putchar(s[k]) == -1)
				return (-1);
			out++;
		}
		if (f.f_minus && pad)
		{
			k = padn(' ', pad);
			if (k == -1)
				return (-1);
			out += k;
		}
		return (out);
	}

	/* ---------- %c ---------- */
	if (f.spec == 'c')
	{
		int ch = va_arg(*ap, int);
		int pad = (f.width > 1) ? f.width - 1 : 0;
		int k;

		if (!f.f_minus && pad)
		{
			k = padn(' ', pad);
			if (k == -1)
				return (-1);
			out += k;
		}
		if (_putchar((char)ch) == -1)
			return (-1);
		out++;
		if (f.f_minus && pad)
		{
			k = padn(' ', pad);
			if (k == -1)
				return (-1);
			out += k;
		}
		return (out);
	}

	/* ---------- %% ---------- */
	if (f.spec == '%')
	{
		int pad = (f.width > 1) ? f.width - 1 : 0;
		char padc = (!f.f_minus && f.f_zero && f.precision < 0) ? '0' : ' ';
		int k;

		if (!f.f_minus && pad)
		{
			k = padn(padc, pad);
			if (k == -1)
				return (-1);
			out += k;
		}
		if (_putchar('%') == -1)
			return (-1);
		out++;
		if (f.f_minus && pad)
		{
			k = padn(' ', pad);
			if (k == -1)
				return (-1);
			out += k;
		}
		return (out);
	}

	/* ---------- %p ---------- */
	if (f.spec == 'p')
	{
		const void *pv = va_arg(*ap, const void *);
		int pad, k;

		if (!pv)
		{
			const char *nil = "(nil)";
			int slen = 5;

			pad = (f.width > slen) ? f.width - slen : 0;
			if (!f.f_minus && pad)
			{
				k = padn(' ', pad);
				if (k == -1)
					return (-1);
				out += k;
			}
			for (k = 0; k < slen; k++)
			{
				if (_putchar(nil[k]) == -1)
					return (-1);
				out++;
			}
			if (f.f_minus && pad)
			{
				k = padn(' ', pad);
				if (k == -1)
					return (-1);
				out += k;
			}
			return (out);
		}
		else
		{
			char buf[2 + 16];
			int len;

			buf[0] = '0';
			buf[1] = 'x';
			len = 2 + utoa_base((unsigned long)pv, 16, 0, buf + 2);

			pad = (f.width > len) ? f.width - len : 0;
			if (!f.f_minus && pad)
			{
				k = padn(' ', pad);
				if (k == -1)
					return (-1);
				out += k;
			}
			for (k = 0; k < len; k++)
			{
				if (_putchar(buf[k]) == -1)
					return (-1);
				out++;
			}
			if (f.f_minus && pad)
			{
				k = padn(' ', pad);
				if (k == -1)
					return (-1);
				out += k;
			}
			return (out);
		}
	}

	/* ---------- numbers: d i u o x X b ---------- */
	{
		int base, upper, is_signed;
		unsigned long uval;
		long sval;
		int neg, zprec, ndig, width, prec, k;
		int prefix_len, body_len, pad;
		char signch, padc;
		char prefix[2];
		char digits[65];

		base = 10;
		upper = 0;
		is_signed = 0;
		uval = 0UL;
		sval = 0;
		neg = 0;
		zprec = 0;
		ndig = 0;
		width = (f.width > 0) ? f.width : 0;
		prec = f.precision;
		prefix_len = 0;
		signch = 0;
		prefix[0] = 0;
		prefix[1] = 0;

		if (f.spec == 'd' || f.spec == 'i')
		{
			is_signed = 1;
			base = 10;
		}
		else if (f.spec == 'u')
		{
			is_signed = 0;
			base = 10;
		}
		else if (f.spec == 'o')
		{
			is_signed = 0;
			base = 8;
		}
		else if (f.spec == 'x')
		{
			is_signed = 0;
			base = 16;
			upper = 0;
		}
		else if (f.spec == 'X')
		{
			is_signed = 0;
			base = 16;
			upper = 1;
		}
		else if (f.spec == 'b')
		{
			is_signed = 0;
			base = 2;
		}
		else
		{
			/* unknown spec: print literally */
			if (_putchar('%') == -1) return (-1);
			if (_putchar(f.spec) == -1) return (-1);
			return (2);
		}

		/* fetch with length modifiers */
		if (is_signed)
		{
			if (f.length == 2) /* l */
				sval = va_arg(*ap, long);
			else
				sval = va_arg(*ap, int);
			if (f.length == 1) /* h : cast to short */
				sval = (short)sval;

			if (sval < 0)
			{
				neg = 1;
				uval = (unsigned long)(-sval);
			}
			else
			{
				uval = (unsigned long)sval;
				if (f.f_plus) signch = '+';
				else if (f.f_space) signch = ' ';
			}
		}
		else
		{
			if (f.length == 2) /* l */
				uval = va_arg(*ap, unsigned long);
			else
			{
				unsigned int ui = va_arg(*ap, unsigned int);
				if (f.length == 1) /* h */
					uval = (unsigned long)((unsigned short)ui);
				else
					uval = (unsigned long)ui;
			}
		}

		/* convert digits now; may be suppressed by precision rule */
		ndig = utoa_base(uval, base, upper, digits);

		/* precision==0 and value==0 => print nothing
		   EXCEPT %#.0o prints single '0' */
		if (prec == 0 && uval == 0UL)
		{
			if (!(f.spec == 'o' && f.f_hash))
				ndig = 0;
		}

		/* # prefix (non-zero) */
		if (f.f_hash && uval != 0UL)
		{
			if (base == 8)
			{
				prefix[0] = '0';
				prefix_len = 1;
			}
			else if (base == 16)
			{
				prefix[0] = '0';
				prefix[1] = upper ? 'X' : 'x';
				prefix_len = 2;
			}
		}

		if (neg)
			signch = '-';

		/* zeros from precision */
		if (prec > ndig)
			zprec = prec - ndig;

		/* '0' flag ignored when precision specified */
		padc = (!f.f_minus && f.f_zero && prec < 0) ? '0' : ' ';

		body_len = (signch ? 1 : 0) + prefix_len + (ndig + zprec);

		/* left pad */
		pad = (width > body_len) ? width - body_len : 0;
		if (!f.f_minus && pad)
		{
			int wrote = padn(padc, pad);
			if (wrote == -1)
				return (-1);
			out += wrote;
			pad = 0;
		}

		/* sign */
		if (signch)
		{
			if (_putchar(signch) == -1)
				return (-1);
			out++;
		}
		/* prefix */
		if (prefix_len)
		{
			if (_putchar(prefix[0]) == -1)
				return (-1);
			out++;
			if (prefix_len == 2)
			{
				if (_putchar(prefix[1]) == -1)
					return (-1);
				out++;
			}
		}
		/* precision zeros */
		if (zprec)
		{
			int wrote2 = padn('0', zprec);
			if (wrote2 == -1)
				return (-1);
			out += wrote2;
		}
		/* digits */
		for (k = 0; k < ndig; k++)
		{
			if (_putchar(digits[k]) == -1)
				return (-1);
			out++;
		}
		/* right pad (spaces only) */
		if (f.f_minus)
		{
			int rpad = (width > body_len) ? width - body_len : 0;
			if (rpad)
			{
				int wrote3 = padn(' ', rpad);
				if (wrote3 == -1)
					return (-1);
				out += wrote3;
			}
		}

		return (out);
	}
}
