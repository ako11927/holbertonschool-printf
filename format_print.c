#include "main.h"

/* repeat char */
int putnchar(char c, int n)
{
	int k, r; for (k=0;k<n;k++){ r=_putchar(c); if (r<0) return -1; } return n;
}

/* utoa base 2..16 */
static int utoa_base(unsigned long n, int base, int upper, char *buf)
{
	const char *lo="0123456789abcdef", *up="0123456789ABCDEF";
	char tmp[65]; int len=0; if (n==0){ buf[0]='0'; buf[1]='\0'; return 1; }
	while (n){ int v=(int)(n%(unsigned long)base); tmp[len++]=(upper?up[v]:lo[v]); n/=(unsigned long)base; }
	for (int i=0;i<len;i++) buf[i]=tmp[len-1-i]; buf[len]='\0'; return len;
}

/* ints: d i u o x X */
static int print_intlike(const fmt_t *f, va_list *ap)
{
	int base=10, upper=0, is_signed=0;
	unsigned long u=0; long s=0; int neg=0;
	char num[65]; int nlen, zeros=0, pad=0, left=f->f_minus;
	int total=0, use_zero_pad;

	if (f->spec=='d'||f->spec=='i'){ is_signed=1; base=10; }
	else if (f->spec=='u'){ base=10; }
	else if (f->spec=='o'){ base=8; }
	else if (f->spec=='x'||f->spec=='X'){ base=16; upper=(f->spec=='X'); }
	else return -2;

	/* fetch with l/h */
	if (is_signed)
	{
		if (f->length=='l') s=va_arg(*ap,long);
		else if (f->length=='h') s=(short)va_arg(*ap,int);
		else s=va_arg(*ap,int);
		if (s<0){ neg=1; u=(unsigned long)(-s); } else u=(unsigned long)s;
	}
	else
	{
		if (f->length=='l') u=va_arg(*ap,unsigned long);
		else if (f->length=='h') u=(unsigned short)va_arg(*ap,unsigned int);
		else u=va_arg(*ap,unsigned int);
	}

	nlen = utoa_base(u, base, upper, num);
	if (f->precision==0 && u==0) nlen = 0;          /* nothing printed for digits */

	if (f->precision > nlen) zeros = f->precision - nlen;

	use_zero_pad = (f->f_zero && !left && f->precision < 0);

	/* compute pad */
	{
		int signlen = (is_signed && (neg || f->f_plus || f->f_space)) ? 1 : 0;
		int altlen = 0;

		if (f->f_hash && u!=0 && (f->spec=='x'||f->spec=='X')) altlen=2;
		if (f->f_hash && f->spec=='o' && (u!=0 || f->precision==0))
			if (!(f->precision > nlen) && !(nlen>0 && num[0]=='0')) altlen=1;

		int need = signlen + altlen + zeros + nlen;
		if (f->width > need) pad = f->width - need;

		if (!left && !use_zero_pad && pad){ int r=putnchar(' ', pad); if (r<0) return -1; total+=r; pad=0; }

		/* sign */
		if (is_signed){
			if (neg && _putchar('-')<0) return -1;
			if (!neg && f->f_plus && _putchar('+')<0) return -1;
			if (!neg && !f->f_plus && f->f_space && _putchar(' ')<0) return -1;
			if (neg || f->f_plus || f->f_space) total++;
		}

		/* alt hex */
		if (f->f_hash && u!=0 && (f->spec=='x'||f->spec=='X')){
			if (_putchar('0')<0) return -1;
			if (_putchar(f->spec=='x'?'x':'X')<0) return -1;
			total+=2;
		}
		/* alt octal */
		if (f->f_hash && f->spec=='o' && (u!=0 || f->precision==0)){
			if (!(f->precision > nlen) && !(nlen>0 && num[0]=='0')){
				if (_putchar('0')<0) return -1; total+=1;
			}
		}

		if (use_zero_pad && pad){ int r2=putnchar('0', pad); if (r2<0) return -1; total+=r2; pad=0; }
	}

	/* zeros from precision */
	if (zeros){ int r3=putnchar('0', zeros); if (r3<0) return -1; total+=r3; }

	/* digits */
	for (int k=0;k<nlen;k++){ if (_putchar(num[k])<0) return -1; total++; }

	/* left-justify spaces */
	if (left && pad){ int r4=putnchar(' ', pad); if (r4<0) return -1; total+=r4; }

	return total;
}

/* strings/chars/% with width + precision(for strings) */
static int print_strlike(const fmt_t *f, va_list *ap)
{
	int total=0, pad, len, outlen, left=f->f_minus;
	const char *s; char c;

	if (f->spec=='s')
	{
		s = va_arg(*ap, const char *); if (!s) s="(null)";
		for (len=0; s[len]; len++) ;
		outlen = (f->precision>=0 && f->precision<len) ? f->precision : len;
		pad = (f->width>outlen) ? (f->width - outlen) : 0;

		if (!left && pad){ int r=putnchar(' ',pad); if (r<0) return -1; total+=r; }
		for (int i=0;i<outlen;i++){ if (_putchar(s[i])<0) return -1; total++; }
		if (left && pad){ int r2=putnchar(' ',pad); if (r2<0) return -1; total+=r2; }
		return total;
	}
	else if (f->spec=='c' || f->spec=='%')
	{
		c = (f->spec=='%') ? '%' : (char)va_arg(*ap,int);
		pad = (f->width>1) ? (f->width - 1) : 0;
		if (!left && pad){ int r3=putnchar(' ',pad); if (r3<0) return -1; total+=r3; }
		if (_putchar(c)<0) return -1; total++;
		if (left && pad){ int r4=putnchar(' ',pad); if (r4<0) return -1; total+=r4; }
		return total;
	}
	return -2;
}

/* route one specifier */
int print_formatted(const fmt_t *f, va_list *ap)
{
	int r;

	r = print_intlike(f, ap); if (r!=-2) return r;
	r = print_strlike(f, ap); if (r!=-2) return r;

	/* keep your already-implemented custom ones */
	if (f->spec=='b') return print_base((unsigned long)va_arg(*ap,unsigned int),2,0);
	if (f->spec=='p') return print_pointer(va_arg(*ap,void *));
	if (f->spec=='S') return print_S(va_arg(*ap,const char *));
	if (f->spec=='r') return print_rev(va_arg(*ap,const char *));
	if (f->spec=='R') return print_rot13(va_arg(*ap,const char *));

	/* unknown: print literally */
	if (_putchar('%')<0) return -1;
	if (_putchar(f->spec)<0) return -1;
	return 2;
}
