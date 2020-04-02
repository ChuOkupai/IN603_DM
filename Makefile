CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -Ofast
BIN1		:= geffe
BIN1_OBJ	:= .obj/geffe.o .obj/utils.o
BIN1_INC	:= $(BIN1_OBJ:.obj/%.o=inc/%.h)

all: bin/$(BIN1)

clean:
	rm -rf bin .obj

re: clean all

test: $(BIN1)
	#./$< 0b00000000 0b000000000000000100000000000000010000000000000001 2
	./$< 0b00000000 0b011111111111110100000000000000010000000000000001 2

bin:
	mkdir $@

.obj:
	mkdir $@

.obj/geffe.o: src/geffe.c inc/geffe.h inc/utils.h | .obj
	$(CC) $(CFLAGS) -I./inc -c $< -o $@

.obj/utils.o: src/utils.c | obj
	$(CC) $(CFLAGS) -I./inc -c $< -o $@

bin/$(BIN1): src/$(BIN1)-main.c $(BIN1_OBJ) $(BIN1_INC) | bin
	$(CC) $(CFLAGS) -I./inc $< $(BIN1_OBJ) -o $@

.PHONY: all clean re test
