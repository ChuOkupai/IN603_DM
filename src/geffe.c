#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include "geffe.h"
#include "utils.h"

// L0 = 0b0000000010010011 => position: 0, 1, 4, 7
// L1 = 0b0000100010000011 => position: 0, 1, 7, 11
// L2 = 0b0000000000101101 => position: 0, 2, 3, 5
#define FEEDBACK(L, A, B, C) \
	L = (L >> 1) | ((L ^ L >> A ^ L >> B ^ L >> C) << 15)

// Affiche la matrice de permutations (DEBUG)
static void print_F(t_u8 F)
{
	printf("┌─────────┬───┬───┬───┬───┬───┬───┬───┬───┐\n│x0x1x2   │");
	for (t_u8 i = 0; i < 8; ++i)
		printf("%c%c%c│", btoa(i & 0b100), btoa(i & 0b10), btoa(i & 1));
	printf("\n├─────────┼───┼───┼───┼───┼───┼───┼───┼───┤\n│F(x0x1x2)│");
	for (t_u8 i = 0; i < 8; ++i)
		printf("%c  │", btoa(F & (0x80 >> i)));
	printf("\n└─────────┴───┴───┴───┴───┴───┴───┴───┴───┘\n\n");
}

// Affiche un LFSR (DEBUG)
static void print_LI(t_u16 L, int i)
{
	printf("L%d ┌", i);
	for (int j = 16; j > 0; --j)
	{
		putchar('0' + !!(L & 0x8000));
		L <<= 1;
	}
}

// Affiche l'ensemble des LFSRs (DEBUG)
static void print_L(t_u16 *L)
{
	print_LI(L[0], 0);
	printf("──┐\n   └────────┴──┴──┴┘  │\n");
	print_LI(L[1], 1);
	printf("─┐│\n   └────┴───┴─────┴┘ ││\n");
	print_LI(L[2], 2);
	printf("┐││\n   └──────────┴─┴┴─┘│││\n");
}

// Echange le bit à la position i avec celui à la position j
static t_u8 swapb(t_u8 n, t_u8 i, t_u8 j)
{
	t_u8 t = ((n >> i) ^ (n >> j)) & 1;
	return (n ^ ((t << i) | (t << j)));
}

t_u64 generator_init(char **arg, int size, t_generator *g)
{
	if (size != 3) // Pas assez d'arguments
		return (errno = EINVAL);
	// F utilise les permutations : 1 <-> 4 et 3 <-> 6
	// Les index sont stockés dans l'ordre inverse donc:
	// => (7 - 1 = 6) <-> (7 - 4 = 3)
	// => (7 - 3 = 4) <-> (7 - 6 = 1)
	g->F = swapb(swapb(parse(arg[0], UCHAR_MAX), 6, 3), 4, 1);
	t_u64 n = parse(arg[1], 0xffffffffffff);
	g->L[0] = (n >> 32) & 0xffff;
	g->L[1] = (n >> 16) & 0xffff;
	g->L[2] = n & 0xffff;
	return parse(arg[2], ULONG_MAX);
}

// Renvoie la valeur de x = x0x1x2
static t_u8 X(t_generator *g)
{
	return ((g->L[0] & 1) << 2) | ((g->L[1] & 1) << 1) | (g->L[2] & 1);
}

// Renvoie la valeur de F(x0x1x2) en ASCII
static t_u8 F(t_u8 filter, t_u8 x)
{
	return btoa(filter & (0x80 >> x));
}

void generator_run(t_generator *g, t_u64 n, int debug)
{
	if (debug)
		print_F(g->F);
	if (!n)
		return ;
	while (n--)
	{
		if (debug)
			print_L(g->L);
		// x = x0x1x2
		t_u8 x = X(g);
		if (debug)
			printf("%18cF(%c%c%c) = ", ' ', btoa((x >> 2)), btoa(x & 2),
			btoa(x & 1));
		putchar(F(g->F, x)); // F(x0x1x2)
		if (debug)
			putchar('\n');
		FEEDBACK(g->L[0], 1, 4, 7);
		FEEDBACK(g->L[1], 1, 7, 11);
		FEEDBACK(g->L[2], 2, 3, 5);
	}
	if (!debug)
		putchar('\n');
}

// Renvoie 1 si le générateur correspond à la suite s, 0 sinon
int match(t_generator *g, const char *s)
{
	while (*s)
	{
		if (F(g->F, X(g)) != *s++)
			return (0);
		FEEDBACK(g->L[0], 1, 4, 7);
		FEEDBACK(g->L[1], 1, 7, 11);
		FEEDBACK(g->L[2], 2, 3, 5);
	}
	return (1);
}

int is_found(t_generator *g, const char *s, t_u16 i)
{
	if (!s[i] || i > 15)
	{
		t_generator g2 = *g;
		return (match(&g2, s));
	}
	t_u16 z = s[i] - '0', j = 1 << i;
	g->L[0] = (g->L[0] & ~j) | (!z << i);
	g->L[1] = (g->L[1] & ~j) | (z << i);
	g->L[2] = (g->L[2] & ~j) | (z << i);
	if (is_found(g, s, i + 1)) // !zzz
		return (1);
	g->L[0] ^= j;
	g->L[1] ^= j;
	if (is_found(g, s, i + 1)) // z!zz
		return (1);
	g->L[0] ^= j;
	if (is_found(g, s, i + 1)) // !z!zz
		return (1);
	g->L[2] ^= j;
	return (is_found(g, s, i + 1)); // !z!z!z
}

t_generator generator_attack(const char *s)
{
	t_generator	g = { 0 };
	g.F = 0b11010100;

	if (!is_found(&g, s, 0))
		errno = EINVAL; // non trouvé
	return (g);
}
