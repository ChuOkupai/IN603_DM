CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -Ofast
BIN1		:= geffe
BIN1_OBJ	:= .obj/geffe.o .obj/utils.o
BIN1_INC	:= $(BIN1_INC:.obj/%.o=inc/%.h) inc/type.h

# Fonction R(SET, N)
# Génère un texte aléatoire de taille N qui contient que des caratères du SET
# Exemple $(call RAND, '0-9', 1) génère un nombre de taille 1 en base 10
R	= $(shell cat /dev/urandom | tr -dc $(1) | fold -w $(2) | head -n 1)
# Set hexadecimal
HEX	:= '0-9a-f'

all: bin/$(BIN1)

clean:
	rm -rf bin .obj

re: clean all

test: bin/$(BIN1)
	./$< 0 0 1
	./$< 0xff 0 1
	./$< 0xd1 0x19947c7747d6 4
	./$< 0b01011011 0x7e2be09a3261 3

rand-$(BIN1): bin/$(BIN1)
	./$< 0b$(call R, '0-1', 8) 0x$(call R, $(HEX), 12) $(call R, '1-8', 1)

bin:
	mkdir $@

.obj:
	mkdir $@

.obj/geffe.o: src/geffe.c inc/geffe.h inc/utils.h inc/type.h | .obj
	$(CC) $(CFLAGS) -I./inc -c $< -o $@

.obj/utils.o: src/utils.c inc/type.h | .obj
	$(CC) $(CFLAGS) -I./inc -c $< -o $@

bin/$(BIN1): src/$(BIN1)-main.c $(BIN1_OBJ) $(BIN1_INC) | bin
	$(CC) $(CFLAGS) -I./inc $< $(BIN1_OBJ) -o $@

.PHONY: all clean re test rand-$(BIN1)
