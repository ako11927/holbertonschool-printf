#include "main.h"

/* parse %[+ #0-][width][.precision][l|h]<spec> */
int parse_format(const char *fmt, int *i, fmt_t *out)
{
	int j = *i;

	out->f_plus = out->f_space = out->f_hash = 0;
	out->f_zero = out->f_minus = 0;
	out->width = -1;
	out->precision = -1;
	out->length = 0;
	out->spec = 0;

	/* flags */
	for (; fmt[j]=='+'||fmt[j]==' '||fmt[j]=='#'||fmt[j]=='0'||fmt[j]=='-'; j++)
	{
		if (fmt[j]=='+') out->f_plus=1;
		else if (fmt[j]==' ') out->f_space=1;
		else if (fmt[j]=='#') out->f_hash=1;
		else if (fmt[j]=='0') out->f_zero=1;
		else if (fmt[j]=='-') out->f_minus=1;
	}

	/* width (digits only) */
	if (fmt[j]>='0' && fmt[j]<='9')
	{
		int w = 0;
		while (fmt[j]>='0' && fmt[j]<='9') w = w*10 + (fmt[j++]-'0');
		out->width = w;
	}

	/* precision .digits */
	if (fmt[j]=='.')
	{
		int p = 0, saw = 0;
		j++;
		while (fmt[j]>='0' && fmt[j]<='9') { p = p*10 + (fmt[j++]-'0'); saw = 1; }
		out->precision = saw ? p : 0;
	}

	/* length */
	if (fmt[j]=='l' || fmt[j]=='h') out->length = fmt[j++];

	/* spec */
	if (fmt[j]=='\0') return (-1);
	out->spec = fmt[j];
	*i = j;
	return (0);
}
