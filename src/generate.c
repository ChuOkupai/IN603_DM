#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// L0 = 0b0000000010010011 => position: 0, 1, 4, 7
// L1 = 0b0000100010000011 => position: 0, 1, 7, 11
// L2 = 0b0000000000101101 => position: 0, 2, 3, 5
#define FEEDBACK(N, A, B, C) \
	N = (N >> 1) | ((N ^ N >> A ^ N >> B ^ N >> C) << 15)

void print_L(uint16_t *L)
{
	uint16_t n;

	for (int i = 0; i < 3; ++i)
	{
		printf("L%d: ", i);
		n = L[i];
		for (int j = 8; j > 0; --j)
		{
			putchar('0' + !!(n & 0x8000));
			n <<= 1;
		}
		putchar(' ');
		for (int j = 8; j > 0; --j)
		{
			putchar('0' + !!(n & 0x8000));
			n <<= 1;
		}
		putchar('\n');
	}
}

void print_F(uint8_t F)
{
	printf("F: ");
	for (int i = 7; i >= 0; --i)
		putchar('0' + !!((F >> i) & 1));
	putchar('\n');
}

// Fonction utilitaire pour quitter en affichant une erreur
static int print_error(const char *bin, const char *message)
{
	fprintf(stderr, "error: %s\n", message);
	fprintf(stderr, "Try '%s --help' for more information.\n", bin);
	return -1;
}

// Fonction utilitaire pour quitter en affichant l'aide
static int print_usage(const char *bin)
{
	printf("usage: %s [F] [K] [n]\n", bin);
	puts("F: 8 bit filter function");
	puts("K: 48 bit key");
	puts("n: maximum bits in octal, decimal or hexadecimal format");
	return 0;
}

// Fonction utilitaire qui vérifie si le caractère c est est chiffre en base 2
static int isdigit2(int c)
{
	return (c == '0' || c == '1');
}

// Echange le bit à la position i avec celui à la position j
static uint8_t swapb(uint8_t n, uint8_t i, uint8_t j)
{
	uint8_t t;

	t = ((n >> i) ^ (n >> j)) & 1;
	return (n ^ ((t << i) | (t << j)));
}

// Initialise F
static uint8_t init_F(char *s)
{
	uint8_t	F;
	int		i;

	F = 0;
	for (i = 0; s[i] && i < 8 && isdigit2(s[i]); ++i)
		F = (F << 1) | (uint8_t)(s[i] - '0');
	if (i != 8 || s[i])
		return (errno = EINVAL);
	// F utilise les permutations : 1 <-> 4 et 3 <-> 6
	// Les index sont dans l'ordre inverse donc:
	// => 7 - 1 <-> 7 - 4
	// => 7 - 3 <-> 7 - 6
	return (swapb(swapb(F, 6, 3), 4, 1));
}

// Initialise les 3 registres L0, L1 et L2 avec la clé K
static void init_L(uint16_t *L, char *s)
{
	int j;

	j = 16;
	for (int i = 0; i < 3 && j == 16; ++i, s += j)
		for (j = 0; s[j] && j < 16 && isdigit2(s[j]); ++j)
			L[i] = (L[i] << 1) | (uint8_t)(s[j] - '0');
	if (j != 16 || *s)
		errno = EINVAL;
}

int main(int ac, char **av)
{
	for (int i = 1; i < ac; ++i)
		if (!strcmp(av[i], "--help"))
			return print_usage(av[0]);
	if (ac != 4)
		return print_error(av[0], strerror(EINVAL));
	uint16_t	L[3];
	uint8_t		F;
	uint64_t	n;
	uint8_t		c;

	F = init_F(av[1]);
	init_L(L, av[2]);
	if (!(n = strtoul(av[3], NULL, 0)))
		errno = EINVAL;
	if (errno) // Gestion d'erreurs éventuelles
		return print_error(av[0], strerror(errno));
	//print_L(L);
	//print_F(F);
	//printf("n: %lu\n", n);
	while (n--)
	{
		//print_L(L);
		c = ((L[0] & 0b100) | (L[1] & 0b10) | (L[2] & 1));
		//printf("x: %u %c%c%c\n", c, (c >> 2) + '0', ((c >> 1) & 1) + '0', (c & 1) + '0');
		putchar('0' + !!(F & (0x80 >> c)));
		FEEDBACK(L[0], 1, 4, 7);
		FEEDBACK(L[1], 1, 7, 11);
		FEEDBACK(L[2], 2, 3, 5);
	}
	putchar('\n');
	return (0);
}
