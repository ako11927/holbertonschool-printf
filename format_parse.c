#include "main.h"

/**
 * parse_format - parse a printf-style conversion after '%'
 * @fmt: full format string
 * @i: index pointer into fmt
 * @out: result holder for flags, width, precision, length and spec
 *
 * Return: 0 on success, -1 on error
 */
int parse_format(const char *fmt, int *i, fmt_t *out)
{
	int j;

	if (fmt == NULL || i == NULL || out == NULL)
		return (-1);

	j = *i;

	out->f_plus = 0;
	out->f_space = 0;
	out->f_hash = 0;
	out->f_zero = 0;
	out->f_minus = 0;
	out->width = -1;
	out->precision = -1;
	out->length = 0;
	out->spec = 0;

	/* flags */
	while (fmt[j] == '+' || fmt[j] == ' ' || fmt[j] == '#' ||
	       fmt[j] == '0' || fmt[j] == '-')
	{
		if (fmt[j] == '+')
		{
			out->f_plus = 1;
		}
		else if (fmt[j] == ' ')
		{
			out->f_space = 1;
		}
		else if (fmt[j] == '#')
		{
			out->f_hash = 1;
		}
		else if (fmt[j] == '0')
		{
			out->f_zero = 1;
		}
		else if (fmt[j] == '-')
		{
			out->f_minus = 1;
		}
		j++;
	}

	/* width (digits only) */
	if (fmt[j] >= '0' && fmt[j] <= '9')
	{
		int w = 0;

		while (fmt[j] >= '0' && fmt[j] <= '9')
		{
			w = (w * 10) + (fmt[j] - '0');
			j++;
		}
		out->width = w;
	}

	/* precision .digits */
	if (fmt[j] == '.')
	{
		int p = 0;
		int saw = 0;

		j++;
		while (fmt[j] >= '0' && fmt[j] <= '9')
		{
			p = (p * 10) + (fmt[j] - '0');
			j++;
			saw = 1;
		}
		out->precision = saw ? p : 0;
	}

	/* length */
	if (fmt[j] == 'l' || fmt[j] == 'h')
	{
		out->length = fmt[j];
		j++;
	}

	/* specifier */
	if (fmt[j] == '\0')
		return (-1);

	out->spec = fmt[j];
	*i = j;
	return (0);
}
