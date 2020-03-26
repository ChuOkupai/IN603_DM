CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror -Ofast
BIN1	:= generate

$(BIN1): src/$(BIN1).c
	$(CC) $(CFLAGS) $< -o $@

all: $(BIN1)

clean:
	rm -rf $(BIN1)

re: clean all

.PHONY: all clean re
