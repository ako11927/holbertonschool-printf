#include "main.h"

size_t strnlen_prec(const char *s, int prec)
{
	size_t n = 0;

	if (s == 0)
		return (0);
	while (*s != '\0' && (prec < 0 || (int)n < prec))
	{
		n++;
		s++;
	}
	return (n);
}
