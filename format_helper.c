#include "main.h"

/**
 * strnlen_prec - strlen with precision cap
 * @s: string (may be NULL)
 * @prec: max chars (-1 => unlimited)
 * Return: length up to prec
 */
size_t strnlen_prec(const char *s, int prec)
{
	size_t n = 0;

	if (!s)
		return (0);

	if (prec < 0) {
		while (s[n])
			n++;
	} else {
		while (s[n] && n < (size_t)prec)
			n++;
	}
	return (n);
}
