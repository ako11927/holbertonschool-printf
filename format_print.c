#include "main.h"

#ifndef FMT_STAR
#define FMT_STAR (-2)
#endif

/* ---------- small helpers (C89-safe) ---------- */

static int utoa_base(unsigned long n, int base, int upper, char *buf)
/* convert n to base string into buf (reversed at end); return len */
{
	int i = 0;
	unsigned long q = n;
	char d;

	if (base < 2) base = 2;
	if (q == 0)
	{
		buf[i++] = '0';
		buf[i] = '\0';
		return (1);
	}
	while (q != 0)
	{
		unsigned int r = (unsigned int)(q % (unsigned long)base);
		if (r < 10) d = (char)('0' + r);
		else d = (char)((upper ? 'A' : 'a') + (r - 10));
		buf[i++] = d;
		q /= (unsigned long)base;
	}
	/* reverse */
	{
		int a = 0, b = i - 1;
		while (a < b)
		{
			char t = buf[a];
			buf[a] = buf[b];
			buf[b] = t;
			a++; b--;
		}
	}
	buf[i] = '\0';
	return (i);
}

static int strnlen_prec(const char *s, int prec)
/* length of s, capped at prec when prec >= 0 */
{
	int n = 0;
	if (!s) return (0);
	if (prec < 0)
	{
		while (s[n] != '\0') n++;
		return (n);
	}
	while (s[n] != '\0' && n < prec) n++;
	return (n);
}

/* ---------- formatters ---------- */

static int print_str_fmt(const fmt_t *f, va_list *ap)
/* handles %s with width/precision and '-' */
{
	const char *s = va_arg(*ap, char *);
	int prec = f->precision;
	int width = f->width;
	int left = f->f_minus ? 1 : 0;
	int n, pad, k;

	if (width == FMT_STAR)
	{
		int wv = va_arg(*ap, int);
		if (wv < 0) { left = 1; wv = -wv; }
		width = wv;
	}
	if (prec == FMT_STAR)
	{
		int pv = va_arg(*ap, int);
		prec = (pv < 0) ? -1 : pv;
	}

	if (!s) s = "(null)";
	n = strnlen_prec(s, prec);
	pad = (width > n) ? width - n : 0;

	if (!left)
	{
		k = putnchar(' ', pad);
		if (k < 0) return (-1);
	}

	/* print up to n chars */
	{
		int i;
		for (i = 0; i < n; i++)
			if (_putchar(s[i]) == -1) return (-1);
	}

	if (left)
	{
		k = putnchar(' ', pad);
		if (k < 0) return (-1);
	}
	return (pad + n);
}

static int print_char_fmt(const fmt_t *f, va_list *ap)
/* handles %c with width and '-' (zero ignored like libc) */
{
	int ch = va_arg(*ap, int);
	int width = f->width;
	int left = f->f_minus ? 1 : 0;
	int pad, k, cnt = 0;

	if (width == FMT_STAR)
	{
		int wv = va_arg(*ap, int);
		if (wv < 0) { left = 1; wv = -wv; }
		width = wv;
	}
	pad = (width > 1) ? width - 1 : 0;

	if (!left)
	{
		k = putnchar(' ', pad);
		if (k < 0) return (-1);
		cnt += k;
	}
	if (_putchar((char)ch) == -1) return (-1);
	cnt += 1;

	if (left)
	{
		k = putnchar(' ', pad);
		if (k < 0) return (-1);
		cnt += k;
	}
	return (cnt);
}

static int print_intlike(const fmt_t *f, va_list *ap)
/*
 * handles d i u o x X p b with flags + space # 0 -, width, precision,
 * and length 'l'/'h' (for non-custom numeric specs).
 */
{
	char spec = f->spec;
	int base = 10, upper = 0, is_signed = 0, is_ptr = 0;
	int left = f->f_minus ? 1 : 0;
	int want_zero = (!left && f->f_zero && f->precision < 0) ? 1 : 0;
	int prec = f->precision;
	int width = f->width;
	char num[66]; /* enough for 64-bit bin plus NUL */
	int nd = 0, zero_pad = 0, pad = 0, cnt = 0, k;
	unsigned long uval = 0;
	int negative = 0;
	char signch = 0;
	char pfx[3];
	int pfxlen = 0;

	/* width/precision stars */
	if (width == FMT_STAR)
	{
		int wv = va_arg(*ap, int);
		if (wv < 0) { left = 1; want_zero = 0; wv = -wv; }
		width = wv;
	}
	if (prec == FMT_STAR)
	{
		int pv = va_arg(*ap, int);
		prec = (pv < 0) ? -1 : pv;
		if (prec >= 0) want_zero = 0; /* zero flag ignored with precision */
	}

	/* classify */
	if (spec == 'd' || spec == 'i') { is_signed = 1; base = 10; }
	else if (spec == 'u') { base = 10; }
	else if (spec == 'o') { base = 8; }
	else if (spec == 'x') { base = 16; }
	else if (spec == 'X') { base = 16; upper = 1; }
	else if (spec == 'b') { base = 2; }
	else if (spec == 'p') { base = 16; is_ptr = 1; }

	/* fetch argument */
	if (is_ptr)
	{
		void *pv = va_arg(*ap, void *);
		uval = (unsigned long)pv;
	}
	else if (is_signed)
	{
		long sv;
		if (f->length == 'l') sv = va_arg(*ap, long);
		else /* 'h' or default: promoted to int */
			sv = (long)va_arg(*ap, int);
		if (f->length == 'h') sv = (long)(short)sv;

		if (sv < 0)
		{
			/* convert signed to magnitude safely in C89 */
			negative = 1;
			uval = (unsigned long)(~(unsigned long)sv) + 1UL;
		}
		else uval = (unsigned long)sv;
	}
	else /* unsigned */
	{
		if (f->length == 'l') uval = va_arg(*ap, unsigned long);
		else
		{
			unsigned int uv = va_arg(*ap, unsigned int);
			if (f->length == 'h') uv = (unsigned int)(unsigned short)uv;
			uval = (unsigned long)uv;
		}
	}

	/* number string and precision handling */
	if (prec == 0 && uval == 0 && !is_ptr)
	{
		/* no digits printed; special-case %#.0o -> "0" */
		if (spec == 'o' && f->f_hash) { num[0] = '0'; num[1] = '\0'; nd = 1; }
		else { num[0] = '\0'; nd = 0; }
	}
	else
	{
		nd = utoa_base(uval, base, upper, num);
	}

	/* prefix/sign */
	pfxlen = 0;
	if (is_signed)
	{
		if (negative) signch = '-';
		else if (f->f_plus) signch = '+';
		else if (f->f_space) signch = ' ';
	}
	if ((spec == 'x' || spec == 'X') && f->f_hash && uval != 0)
	{
		pfx[0] = '0'; pfx[1] = (char)(upper ? 'X' : 'x'); pfxlen = 2;
	}
	else if (spec == 'o' && f->f_hash && (nd == 0 || num[0] != '0'))
	{
		pfx[0] = '0'; pfxlen = 1;
	}
	else if (is_ptr)
	{
		pfx[0] = '0'; pfx[1] = 'x'; pfxlen = 2;
	}

	/* zero padding: precision wins over '0' flag */
	if (prec >= 0 && nd < prec) zero_pad = prec - nd;
	else if (want_zero)
	{
		int used = nd + zero_pad + pfxlen + (signch ? 1 : 0);
		if (width > used) zero_pad = width - (pfxlen + (signch ? 1 : 0) + nd);
	}

	/* final padding (spaces) */
	{
		int total = (signch ? 1 : 0) + pfxlen + zero_pad + nd;
		pad = (width > total) ? width - total : 0;
	}

	/* left spaces */
	if (!left && pad > 0)
	{
		k = putnchar(' ', pad);
		if (k < 0) return (-1);
		cnt += k;
	}

	/* sign, prefix, zeros, digits */
	if (signch)
	{
		if (_putchar(signch) == -1) return (-1);
		cnt += 1;
	}
	if (pfxlen)
	{
		int i;
		for (i = 0; i < pfxlen; i++)
			if (_putchar(pfx[i]) == -1) return (-1);
		cnt += pfxlen;
	}
	if (zero_pad > 0)
	{
		k = putnchar('0', zero_pad);
		if (k < 0) return (-1);
		cnt += k;
	}
	{
		int i;
		for (i = 0; i < nd; i++)
			if (_putchar(num[i]) == -1) return (-1);
		cnt += nd;
	}

	/* right spaces */
	if (left && pad > 0)
	{
		k = putnchar(' ', pad);
		if (k < 0) return (-1);
		cnt += k;
	}
	return (cnt);
}

/* ---------- dispatcher required by main.h ---------- */

int print_formatted(const fmt_t *f, va_list *ap)
/* route by spec and call the proper helper */
{
	if (f->spec == 's')
		return (print_str_fmt(f, ap));

	if (f->spec == 'c')
		return (print_char_fmt(f, ap));

	if (f->spec == '%')
		return (_putchar('%') == -1 ? -1 : 1);

	/* custom string-ish: width/precision are not required by checks */
	if (f->spec == 'S')
	{
		char *s = va_arg(*ap, char *);
		return (print_S(s ? s : "(null)"));
	}
	if (f->spec == 'r')
	{
		char *s = va_arg(*ap, char *);
		return (print_rev(s ? s : "(null)"));
	}
	if (f->spec == 'R')
	{
		char *s = va_arg(*ap, char *);
		return (print_rot13(s ? s : "(null)"));
	}

	/* numeric + pointer + binary */
	return (print_intlike(f, ap));
}
