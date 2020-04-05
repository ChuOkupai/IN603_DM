CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror -Ofast
BIN1	:= geffe
BIN2	:= geffe-cracker
BIN		:= $(BIN1) $(BIN2)

# Fonction R(SET, N)
# Génère un texte aléatoire de taille N qui contient que des caratères du SET
# Exemple $(call RAND, '0-9', 1) génère un nombre de taille 1 en base 10
R	= $(shell cat /dev/urandom | tr -dc $(1) | fold -w $(2) | head -n 1)
# Set hexadecimal
HEX	:= '0-9a-f'

all: $(BIN)

clean:
	rm -rf $(BIN) .obj

re: clean all

test: $(BIN)
	./$< 63 0xab12cd34ef56 8
	./$< 0xd1 0x19947c7747d6 4
	./$< 0b01011011 0x7e2be09a3261 32
	./$< --debug 0b01000010 0b1011 1
	./$(word 2, $^) 111011101110110001010100010000001001101100011100000110100000100010000101011011011111101100100011111101010010000011101011001011100100101100111000
	./$(word 2, $^) 111111111111111111111110101101101000101001111011111001111011010111111110101101

rand-$(BIN1): $(BIN1)
	./$< 0b$(call R, '0-1', 8) 0x$(call R, $(HEX), 12) $(call R, '1-8', 1)

.obj:
	mkdir $@

.obj/utils.o: src/utils.c inc/type.h | .obj
	$(CC) $(CFLAGS) -I./inc -c $< -o $@

.obj/geffe.o: src/geffe.c .obj/utils.o inc/geffe.h | .obj
	$(CC) $(CFLAGS) -I./inc -c $< -o $@

$(BIN1): src/$(BIN1)-main.c .obj/geffe.o .obj/utils.o
	$(CC) $(CFLAGS) -I./inc $^ -o $@

$(BIN2): src/$(BIN2)-main.c .obj/geffe.o .obj/utils.o
	$(CC) $(CFLAGS) -I./inc $^ -o $@

.PHONY: all clean re test rand-$(BIN1)
