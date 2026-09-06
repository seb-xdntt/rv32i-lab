CC = gcc
CFLAGS = -Wall -Wextra -I ../sim/include

test: ../sim/src/decoder.c ../sim/src/cpu_state.c ../tests/c/decoder_entry/test_decoder.c
	$(CC) $(CFLAGS) $^ -o test_decoder
	./test_decoder