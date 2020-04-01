CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror -Ofast
BIN1	:= generate

$(BIN1): src/$(BIN1).c
	$(CC) $(CFLAGS) $< -o $@

all: $(BIN1)

clean:
	rm -rf $(BIN1)

re: clean all

test: $(BIN1)
	#./$< 0b00000000 0b000000000000000100000000000000010000000000000001 2
	./$< 0b00000000 0b011111111111110100000000000000010000000000000001 2

.PHONY: all clean re test
