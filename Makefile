CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -Wvla -pedantic

LDFLAGS=-fsanitize=address
LDLIBS=-lcriterion

all: tinyprintf

tinyprintf: src/tinyprintf.c src/main.c
	$(CC) $(CFLAGS) -o tinyprintf $^

check: testsuite
	./testsuite

testsuite:
	$(CC) -o $@ tests/test.c src/tinyprintf.c $(LDFLAGS) $(LDLIBS)

clean:
	${RM} -f src/*.o *.a testsuite tinyprintf

.PHONY: all clean check
