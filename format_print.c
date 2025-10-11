#include "main.h"

/* write character c, n times */
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

/**
 * print_formatted - print one parsed conversion
 * @f_in: parsed format (flags/width/precision/length/spec)
 * @ap:   varargs
 * Return: number of chars written, or -1 on error
 */
int print_formatted(const fmt_t *f_in, va_list *ap)
{
	fmt_t f = *f_in;
	int out = 0;

	/* normalize negative width: treat as '-' and make width positive */
	if (f.width < 0)
	{
		f.f_minus = 1;
		f.width = -f.width;
	}

	/* %c */
	if (f.spec == 'c')
	{
		int ch = va_arg(*ap, int);
		int pad = (f.width > 1) ? f.width - 1 : 0;
		int k;

		if (!f.f_minus && pad > 0)
		{
			k = padn(' ', pad);
			if (k == -1)
				return (-1);
			out += k;
		}
		if (_putchar((char)ch) == -1)
			return (-1);
		out++;
		if (f.f_minus && pad > 0)
		{
			k = padn(' ', pad);
			if (k == -1)
				return (-1);
			out += k;
		}
		return (out);
	}
	/* %s */
	else if (f.spec == 's')
	{
		const char *s = va_arg(*ap, char *);
		int slen, pad, k;

		if (!s)
			s = "(null)";

		if (f.precision == 0)
			slen = 0;
		else if (f.precision > 0)
			slen = (int)strnlen_prec(s, f.precision);
		else
			slen = (int)strnlen_prec(s, -1);

		pad = (f.width > slen) ? (f.width - slen) : 0;

		if (!f.f_minus && pad > 0)
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
		if (f.f_minus && pad > 0)
		{
			k = padn(' ', pad);
			if (k == -1)
				return (-1);
			out += k;
		}
		return (out);
	}
	/* %% */
	else if (f.spec == '%')
	{
		if (_putchar('%') == -1)
			return (-1);
		return (1);
	}
	/* %p */
	else if (f.spec == 'p')
	{
		void *pv = va_arg(*ap, void *);

		return (print_pointer(pv));
	}
	/* custom %S, %r, %R, %b */
	else if (f.spec == 'S')
	{
		const char *s = va_arg(*ap, char *);

		return (print_S(s));
	}
	else if (f.spec == 'r')
	{
		const char *s = va_arg(*ap, char *);

		return (print_rev(s));
	}
	else if (f.spec == 'R')
	{
		const char *s = va_arg(*ap, char *);

		return (print_rot13(s));
	}
	else if (f.spec == 'b')
	{
		unsigned int bv = va_arg(*ap, unsigned int);

		return (print_base((unsigned long)bv, 2, 0));
	}
	/* numbers: d i u o x X */
	else if (f.spec == 'd' || f.spec == 'i' || f.spec == 'u' ||
		 f.spec == 'o' || f.spec == 'x' || f.spec == 'X')
	{
		int base = 10, upper = 0;
		unsigned long uval = 0;
		long sval = 0;
		int is_signed = (f.spec == 'd' || f.spec == 'i');
		char signch = 0;
		char digits_buf[64];
		int ndig = 0;
		int prec = f.precision;
		int zeros, body, pad, kpad, k;
		char padc = ' ';

		if (f.spec == 'o')
			base = 8;
		else if (f.spec == 'x' || f.spec == 'X')
		{
			base = 16;
			upper = (f.spec == 'X');
		}

		/* length and value fetch */
		if (is_signed)
		{
			if (f.length == 2)
				sval = va_arg(*ap, long);
			else
				sval = (long)va_arg(*ap, int);
			if (f.length == 1)
				sval = (long)(short)sval;

			if (sval < 0)
			{
				signch = '-';
				uval = (unsigned long)(-sval);
			}
			else
			{
				if (f.f_plus)
					signch = '+';
				else if (f.f_space)
					signch = ' ';
				uval = (unsigned long)sval;
			}
		}
		else
		{
			if (f.length == 2)
				uval = va_arg(*ap, unsigned long);
			else
			{
				unsigned int tmp = va_arg(*ap, unsigned int);

				if (f.length == 1)
					uval = (unsigned long)(unsigned short)tmp;
				else
					uval = (unsigned long)tmp;
			}
		}

		/* digits (respect precision==0 special) */
		if (uval == 0UL)
		{
			if (prec == 0 && !(f.spec == 'o' && f.f_hash))
				ndig = 0;
			else
				digits_buf[ndig++] = '0';
		}
		else
		{
			unsigned long t = uval;

			while (t != 0UL && ndig < (int)sizeof(digits_buf))
			{
				unsigned long d = t % (unsigned long)base;
				char c = (char)((d < 10) ? ('0' + d) :
					(upper ? ('A' + (d - 10)) : ('a' + (d - 10))));
				digits_buf[ndig++] = c;
				t /= (unsigned long)base;
			}
		}

		/* precision zeros; ignore '0' flag if precision present */
		if (prec >= 0)
		{
			if (ndig < prec)
				zeros = prec - ndig;
			else
				zeros = 0;
			padc = ' ';
		}
		else
		{
			zeros = 0;
			if (f.f_zero && !f.f_minus)
				padc = '0';
		}

		/* width/padding (hex alternate was not prefixed; octal handled by digits) */
		body = ndig + zeros + (signch ? 1 : 0);
		pad = (f.width > body) ? (f.width - body) : 0;

		if (!f.f_minus && pad > 0 && padc == ' ')
		{
			kpad = padn(' ', pad);
			if (kpad == -1)
				return (-1);
			out += kpad;
			pad = 0;
		}
		if (signch)
		{
			if (_putchar(signch) == -1)
				return (-1);
			out++;
		}
		if (!f.f_minus && pad > 0 && padc == '0')
		{
			kpad = padn('0', pad);
			if (kpad == -1)
				return (-1);
			out += kpad;
			pad = 0;
		}
		if (zeros > 0)
		{
			kpad = padn('0', zeros);
			if (kpad == -1)
				return (-1);
			out += kpad;
		}

		/* digits (suppressed only when prec==0 and value==0 and not %#o) */
		if (!(ndig == 0 && prec == 0 && uval == 0UL && !(f.spec == 'o' && f.f_hash)))
		{
			for (k = ndig - 1; k >= 0; k--)
			{
				if (_putchar(digits_buf[k]) == -1)
					return (-1);
				out++;
			}
		}

		if (f.f_minus && pad > 0)
		{
			kpad = padn(' ', pad);
			if (kpad == -1)
				return (-1);
			out += kpad;
		}
		return (out);
	}

	/* unknown specifier -> print literally */
	if (_putchar('%') == -1)
		return (-1);
	out++;
	if (_putchar(f.spec) == -1)
		return (-1);
	out++;
	return (out);
}
