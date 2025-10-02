#include "main.h"

/**
 * _putchar - writes a character to stdout
 * @c: character to print
 *
 * Return: 1 on success, -1 on error (errno set)
 */
int _putchar(char c)
{
	int r = write(1, &c, 1);

	if (r == 1)
		return (1);
	return (-1);
}

/**
 * print_char - wrapper over _putchar
 * @c: character to print
 *
 * Return: number of chars printed or -1 on error
 */
int print_char(char c)
{
	return (_putchar(c));
}
