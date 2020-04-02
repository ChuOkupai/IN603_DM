#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include "geffe.h"
#include "utils.h"

#if DEBUG
	// Affiche un LFSR
	static void print_LI(uint16_t L, int i)
	{
		printf("L%d ┌", i);
		for (int j = 16; j > 0; --j)
		{
			putchar('0' + !!(L & 0x8000));
			L <<= 1;
		}
		putchar('\n');
	}

	// Affiche l'ensemble des LFSRs
	static void print_L(uint16_t *L)
	{
		print_LI(L[0], 0);
		printf("   └────────┴──┴──┴┘\n");
		print_LI(L[1], 1);
		printf("   └────┴───┴─────┴┘\n");
		print_LI(L[2], 2);
		printf("   └──────────┴─┴┴─┘\n");
	}

	// Affiche le masque de permutations
	static void print_F(uint8_t F)
	{
		printf("F ");
		for (int i = 7; i >= 0; --i)
			putchar('0' + !!((F >> i) & 1));
		putchar('\n');
	}
#endif

// Echange le bit à la position i avec celui à la position j
static uint8_t swapb(uint8_t n, uint8_t i, uint8_t j)
{
	uint8_t t = ((n >> i) ^ (n >> j)) & 1;
	return (n ^ ((t << i) | (t << j)));
}

uint64_t generator_init(int ac, char **av, t_generator *g)
{
	if (ac != 3)
		return (errno = EINVAL);
	// F utilise les permutations : 1 <-> 4 et 3 <-> 6
	// Les index sont stockés dans l'ordre inverse donc:
	// => (7 - 1 = 6) <-> (7 - 4 = 3)
	// => (7 - 3 = 4) <-> (7 - 6 = 1)
	g->F = swapb(swapb(parse(av[0], UCHAR_MAX), 6, 3), 4, 1);
	uint64_t n = parse(av[1], 0xffffffffffff);
	g->L[0] = (n >> 32) & 0xffff;
	g->L[1] = (n >> 16) & 0xffff;
	g->L[2] = n & 0xffff;
	return parse(av[2], ULONG_MAX);
}

// L0 = 0b0000000010010011 => position: 0, 1, 4, 7
// L1 = 0b0000100010000011 => position: 0, 1, 7, 11
// L2 = 0b0000000000101101 => position: 0, 2, 3, 5
#define FEEDBACK(N, A, B, C) \
	N = (N >> 1) | ((N ^ N >> A ^ N >> B ^ N >> C) << 15)

void generator_run(t_generator *g, uint64_t n)
{
	uint8_t x;

	#if DEBUG
		print_F(g->F);
		printf("n %lu\n", n);
	#endif
	while (n--)
	{
		#if DEBUG
			print_L(g->L);
		#endif
		x = (g->L[0] << 2 | g->L[1] << 1 | g->L[2]) & 0b111; // x = x0x1x2
		x = !!(g->F & (0x80 >> x)); // x = F(x0x1x2)
		putchar('0' + x);
		#if DEBUG
			putchar('\n');
		#endif
		FEEDBACK(g->L[0], 1, 4, 7);
		FEEDBACK(g->L[1], 1, 7, 11);
		FEEDBACK(g->L[2], 2, 3, 5);
	}
	putchar('\n');
}
