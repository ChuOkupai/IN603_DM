#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** L0 = 0b0000000010010011 => bits position: 0, 1, 4, 7
** L1 = 0b0000100010000011 => bits position: 0, 1, 7, 11
** L2 = 0b0000000000101101 => bits position: 0, 2, 3, 5
*/
#define FEEDBACK(N, A, B, C) \
	N = (N >> 1) | ((N ^ N >> A ^ N >> B ^ N >> C) << 15)

typedef struct s_generator
{
	uint16_t	L[3];
	uint8_t		F;
}	t_generator;

// Fonction utilitaire pour afficher des informations
int		print_usage(const char *bin, int ret)
{
	printf("usage: %s [F] [K] [n]\n", bin);
	puts("F: 8 bit filter function");
	puts("K: 48 bit key");
	puts("n: maximum bits in octal, decimal or hexadecimal format");
	return (ret);
}

// Echange le bit à la position i avec celui à la position j
uint8_t	swapb(uint8_t n, uint8_t i, uint8_t j)
{
	uint8_t t;

	t = (n >> i ^ n >> j) & 1;
	return (n ^ ((t << i) | (t << j)));
}

int		init_F(t_generator *g, char *s)
{
	int i;

	for (i = 0; s[i] && i < 8 && (s[i] == '0' || s[i] == '1'); ++i)
		g->F = (g->F << 1) | (uint8_t)(s[i] - '0');
	if (i != 8 || s[i])
		return (-1);
	// F utilise les permutations : 1 <=> 4 et 3 <=> 6
	g->F = (g->F & 0b10100101) | swapb(g->F, 1, 4) | swapb(g->F, 3, 6);
	return (0);
}

int		init_L(t_generator *g, char *s)
{
	int j;

	for (int i = 0; i < 3; ++i)
	{
		for (j = 0; s[j] && j < 16 && (s[j] == '0' || s[j] == '1'); ++j)
			g->L[i] = (g->L[i] << 1) | (uint8_t)(s[j] - '0');
		if (j != 16)
			return (-1);
		s += 16;
	}
	return (-(*s != 0));
}

void	print_L(uint16_t *L)
{
	uint16_t n;

	for (int i = 0; i < 3; ++i)
	{
		printf("L%d ", i);
		n = L[i];
		for (int j = 8; j > 0; --j)
		{
			putchar('0' + ((n & 0x8000) != 0));
			n <<= 1;
		}
		putchar(' ');
		for (int j = 8; j > 0; --j)
		{
			putchar('0' + ((n & 0x8000) != 0));
			n <<= 1;
		}
		putchar(i == 2 ? '\n' : ' ');
	}
}

int		main(int ac, char **av)
{
	t_generator	g;
	uint64_t	n;

	for (int i = 1; i < ac; ++i)
		if (!strcmp(av[i], "--help"))
			return print_usage(av[0], 0);
	if (ac != 4 || init_F(&g, av[1]) < 0 || init_L(&g, av[2]) < 0
		|| (n = strtoul(av[3], 0, 0)) < 1)
		return print_usage(av[0], -1);
	printf("L: %04x %04x %04x\n", g.L[0], g.L[1], g.L[2]);
	printf("F: %02x\n", g.F);
	printf("n: %lu\n", n);
	while (n--)
	{
		printf("n: %c\n", '0' + ((g.L[0] ^ g.L[1] ^ g.L[2]) & 1));
		print_L(g.L);
		FEEDBACK(g.L[0], 1, 4, 7);
		FEEDBACK(g.L[1], 1, 7, 11);
		FEEDBACK(g.L[2], 2, 3, 5);
	}
	return (0);
}
