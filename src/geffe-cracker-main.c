#include <errno.h>
#include <stdio.h>
#include "geffe.h"
#include "utils.h"

#define USAGE "[ENCRYPTION SUITE]"
#define DESC \
	"Geffe generator composed of 3 x 16 bit LFSR\n" \
	"Perform an attack on the generator.\n" \
	"The filter function is set to 0b10001110.\n\n" \
	"  [ENCRYPTION SUITE]  bits to use to find the key\n" \
	"  --help              display this help and exit\n"

int main(int ac, char **av)
{
	if (check_help(ac, av, USAGE, DESC))
		return (0);
	if (ac != 2 || !av[1][0])
		errno = EINVAL;
	else
		for (size_t i = 0; av[1][i]; ++i)
			if (av[1][i] != '0' && av[1][i] != '1')
			{
				errno = EINVAL;
				break ;
			}
	if (errno) // Gestion d'erreurs éventuelles
		return print_error(av[0]);
	t_generator g = generator_attack(av[1]);
	if (errno)
		return print_error(av[0]);
	printf("kO 0x%04hx\n", g.L[0]);
	printf("k1 0x%04hx\n", g.L[1]);
	printf("k2 0x%04hx\n", g.L[2]);
	return (0);
}
