CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -I ./sim/include

test: ./sim/src/decoder.c ./sim/src/cpu_state.c ./sim/src/alu.c ./tests/c/cpu_simulator/test_alu.c
	$(CC) $(CFLAGS) $^ -o test_alu
	./test_alu