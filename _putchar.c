#include "main.h"

/**
 * _putchar - writes a character to stdout
 * @c: character to print
 *
 * Return: 1 on success, -1 on error (errno is set appropriately)
 */
int _putchar(char c)
{
    int r = write(1, &c, 1);

    if (r == 1)
        return (1);
    return (-1);
}
