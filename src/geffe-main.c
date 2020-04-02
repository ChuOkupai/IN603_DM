#include <errno.h>
#include <stdio.h>
#include "geffe.h"
#include "utils.h"

void print_help(char *bin)
{
	printf("usage: %s [FILTER] [KEY] [LEN]\n", bin);
	puts("Each argument can be in base 2 if preceded by '0b',"
	" in base 16 if preceded by '0x' or in base 10.\n");
	puts("FILTER: 8 bits used by the filter function");
	puts("KEY: 48 bits key to initialized LFSRs");
	puts("LEN: maximum bits to output");
}

int main(int ac, char **av)
{
	t_generator	g;
	uint64_t	n;

	if (check_help(ac, av, &print_help))
		return (0);
	n = generator_init(ac - 1, av + 1, &g);
	if (errno) // Gestion d'erreurs éventuelles
		return print_error(av[0]);
	if (n) // Si il y a des bits à afficher
		generator_run(&g, n);
	return (0);
}
