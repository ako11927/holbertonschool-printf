#include "main.h"
#include <unistd.h>

int _putchar(char c)
{
    static char buf[1024];
    static int idx;
    int wrote = 0;

    /* Flush trigger: pass c == 0 */
    if (c == 0) {
        if (idx > 0) {
            wrote = write(1, buf, idx);
            idx = 0;
            (void)wrote; /* ignore in this project; _printf tracks counts */
        }
        return 0;
    }

    buf[idx++] = c;

    if (idx == (int)sizeof(buf)) {
        wrote = write(1, buf, idx);
        idx = 0;
        (void)wrote;
    }
    return 1; /* we buffered one char */
}
