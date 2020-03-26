#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_generator
{
	uint16_t	L[3];
	uint16_t	K[3];
	uint8_t		F;
}	t_generator;

// Echange le bit à la position i avec celui à la position j
uint8_t	swap_bits(uint8_t n, uint8_t i, uint8_t j)
{
	uint8_t t;

	t = (n >> i ^ n >> j) & 1;
	return (n ^ ((t << i) | (t << j)));
}

int		init_F(t_generator *g, char *s)
{
	int i;

	for (i = 0; s[i] && i < 8 && (s[i] == '0' || s[i] == '1'); ++i)
		g->F = (g->F << 1) | (s[i] - '0');
	if (i != 8 || s[i])
		return (-1);
	// F utilise les permutations : 1 <=> 4 et 3 <=> 6
	g->F = (g->F & 0b10100101) | swap_bits(g->F, 1, 4) | swap_bits(g->F, 3, 6);
	return (0);
}

int	init_K(t_generator *g, char *s)
{
	int j;

	for (int i = 0; i < 3; ++i)
	{
		for (j = 0; s[j] && j < 16 && (s[j] == '0' || s[j] == '1'); ++j)
			g->K[i] = (g->K[i] << 1) | (s[j] - '0');
		if (j != 16)
			return (-1);
		s += 16;
	}
	return (-(*s != 0));
}

int	init_generator(t_generator *g, char **av)
{
	memset(g, 0, sizeof(t_generator));
	if (init_F(g, av[1]) < 0 || init_K(g, av[2]) < 0)
		return (-1);
	g->L[0] = 0b0000000010010011;
	g->L[1] = 0b0000100010000011;
	g->L[2] = 0b0000000000101101;
	return (0);
}

int	main(int ac, char **av)
{
	t_generator	g;
	int			n;

	if (ac != 4 || init_generator(&g, av) < 0)
	{
		printf("usage: %s [F] [K] [n]\n", av[0]);
		return (-1);
	}
	n = atoi(av[3]);
	(void)n;
	printf("K: %04x %04x %04x\n", g.K[0], g.K[1], g.K[2]);
	printf("F: %02x\n", g.F);
	return (0);
}
