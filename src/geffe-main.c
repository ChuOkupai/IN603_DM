#include <errno.h>
#include "geffe.h"
#include "utils.h"

#define USAGE "[FILTER] [KEY] [LEN]"
#define DESC \
	"Geffe generator composed of 3 x 16 bit LFSR\n" \
	"Each argument can be in base 2 if preceded by 0b," \
	" in base 16 if preceded by 0x or in base 10.\n\n" \
	"  [FILTER]  8 bits used by the filter function\n" \
	"  [KEY]     48 bits key to initialized LFSRs\n" \
	"  [LEN]     maximum bits to output\n\n" \
	"  --debug   displays system status information\n" \
	"  --help    display this help and exit\n"

int main(int ac, char **av)
{
	t_generator	g;
	t_u64		n;
	int			debug;

	if (check_help(ac, av, USAGE, DESC))
		return (0);
	debug = check_expr(&ac, av, "--debug");
	n = generator_init(av + 1, ac - 1, &g);
	if (errno) // Gestion d'erreurs éventuelles
		return print_error(av[0]);
	generator_run(&g, n, debug);
	return (0);
}
