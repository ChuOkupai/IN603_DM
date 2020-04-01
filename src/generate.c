#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_generator
{
	uint8_t		F; // Le masque de permutations
	uint16_t	L[3]; // Les 3 LFSR
}	t_generator;

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

// Fonction utilitaire qui convertit un character en valeur hexadécimale
// Renvoie 16 si le charactère est invalide
static int xtoi(int c)
{
	if (isdigit(c))
		return (c - '0');
	return ((c -= 'a') >= 0 && c < 16 ? c : 16);
}

/*
** Fonction utilitaire pour récupérer l'entier à partir d'une chaîne de charactères.
** Détecte la base 2 (préfixe 0b), la base 16 (préfixe 0x) et la base 10.
** Si l'argument contient autre chose que des caractères valides,
** errno prend la valeur EINVAL.
** Si le nombre est plus grand que max, errno vaut ERANGE.
*/
static unsigned long parse(const char *s, unsigned long max)
{
	unsigned long	base;
	unsigned long	c;
	unsigned long	n;

	if (!strncmp(s, "0b", 2))
		s += (base = 2);
	else if (!strncmp(s, "0x", 2))
	{
		base = 16;
		s += 2;
	}
	else
		base = 10;
	n = 0;
	do
		if ((c = xtoi(*s)) >= base)
			return (errno = EINVAL);
		else if (n > (max - c) / base)
			return (errno = ERANGE);
		else
			n = n * base + c;
	while (*(++s));
	return (n);
}

// Echange le bit à la position i avec celui à la position j
static uint8_t swapb(uint8_t n, uint8_t i, uint8_t j)
{
	uint8_t t = ((n >> i) ^ (n >> j)) & 1;
	return (n ^ ((t << i) | (t << j)));
}

static int generator_init(int ac, char **av, t_generator *g, uint64_t *n)
{
	if (ac != 4)
		return (errno = EINVAL);
	// F utilise les permutations : 1 <-> 4 et 3 <-> 6
	// Les index sont dans l'ordre inverse donc:
	// => 7 - 1 <-> 7 - 4
	// => 7 - 3 <-> 7 - 6
	g->F = swapb(swapb(parse(av[1], UCHAR_MAX), 6, 3), 4, 1);
	*n = parse(av[2], 281474976710656UL);
	g->L[0] = (*n >> 32) & 0xffff;
	g->L[1] = (*n >> 16) & 0xffff;
	g->L[2] = *n & 0xffff;
	*n = parse(av[3], ULONG_MAX);
	return errno;
}

// Retourne la valeur de sortie du générateur
static uint8_t generator_output(t_generator *g)
{
	uint8_t x = (g->L[0] << 2 | g->L[1] << 1 | g->L[2]) & 0b111; // x = x0x1x2
	return (!!(g->F & (0x80 >> x))); // F(x)
}

// Génère n bits à partir du générateur
static void generator_run(t_generator *g, uint64_t n)
{
	if (!n)
		return ;
	while (n--)
	{
		//print_L(L);
		//printf("x: %u %c%c%c\n", c, (c >> 2) + '0', ((c >> 1) & 1) + '0', (c & 1) + '0');
		putchar('0' + generator_output(g));
		FEEDBACK(g->L[0], 1, 4, 7);
		FEEDBACK(g->L[1], 1, 7, 11);
		FEEDBACK(g->L[2], 2, 3, 5);
	}
	putchar('\n');
}

int main(int ac, char **av)
{
	t_generator	g;
	uint64_t	n;

	for (int i = 1; i < ac; ++i)
		if (!strcmp(av[i], "--help")) // Affichage de l'aide
		{
			printf("usage: %s [FILTER] [KEY] [LEN]\n", av[0]);
			puts("Each argument can be in base 2 if preceded by '0b',"
			" in base 16 if preceded by '0x' or in base 10.\n");
			puts("FILTER: 8 bits used by the filter function");
			puts("KEY: 48 bits key to initialized LFSRs");
			puts("LEN: maximum bits to output");
			return (0);
		}
	if (generator_init(ac, av, &g, &n)) // Gestion d'erreurs éventuelles
	{
		fprintf(stderr, "error: %s\nTry '%s --help' for more information.\n",
		strerror(errno), av[0]);
		return (EXIT_FAILURE);
	}
	print_L(g.L);
	print_F(g.F);
	printf("n: %lu\n", n);
	generator_run(&g, n);
	return (0);
}
