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
	./$< 00000000 000000000000000100000000000000010000000000000001 17

.PHONY: all clean re test
