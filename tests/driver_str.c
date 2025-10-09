#include "main.h"
int main(int argc, char **argv){
    if (argc < 2) return 1;           /* argv[1] = format */
    _printf(argv[1],
            (argc>2?argv[2]:0),
            (argc>3?argv[3]:0),
            (argc>4?argv[4]:0),
            (argc>5?argv[5]:0),
            (argc>6?argv[6]:0),
            (argc>7?argv[7]:0));
    _putchar('\n'); _putchar_flush();
    return 0;
}
