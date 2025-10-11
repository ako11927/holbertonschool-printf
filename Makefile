CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic -std=gnu89
SRC := $(filter-out __t.c, $(wildcard *.c))
OBJ := $(SRC:.c=.o)
LIB := libprintf.a
BIN := tprintf
TEST := __t.c

.PHONY: all clean fclean re run rerun gen_test

all: $(BIN)

$(LIB): $(OBJ)
	ar rcs $@ $^

$(BIN): $(LIB) $(TEST)
	$(CC) $(CFLAGS) $(TEST) -L. -lprintf -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

fclean: clean
	rm -f $(LIB) $(TEST)

re: fclean all

# Generate __t.c (note: single-quoted here-doc keeps \n literally in strings)
gen_test:
	cat > $(TEST) <<'CFILE'
#include "main.h"
#include <stddef.h>
int main(void)
{
    int x = 7;
    _printf("null=%p\n", NULL);
    _printf("local=%p\n", (void *)&x);
    _printf("two=%p %p\n", (void *)&x, (void *)0xABCDEF);
    _printf("mix: str=%s ptr=%p num=%d\n", "hi", (void *)&x, 42);
    return 0;
}
CFILE

run: $(BIN)
	./$(BIN)

# Clean rebuild, (re)generate test, run
rerun: gen_test
	$(MAKE) re
	$(MAKE) run
