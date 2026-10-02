CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -I ./sim/include

test: ./sim/src/cpu_state.c ./sim/src/memory.c ./tests/c/cpu_simulator/test_memory.c
	$(CC) $(CFLAGS) $^ -o test_memory
	./test_memory