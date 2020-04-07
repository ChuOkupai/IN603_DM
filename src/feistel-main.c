#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include "utils.h"

#define USAGE "[x0] [x1]"
#define DESC \
	"Finds the keys k0 and k1 from a couple of clear or encrypted text.\n" \
	"Only 64 bits are needed.\n" \
	"Each argument can be in base 2 if preceded by 0b," \
	" in base 16 if preceded by 0x or in base 10.\n\n" \
	"  [x0]    1st 64 bits block of clear/encrypted text\n" \
	"  [x1]    2nd 64 bits block of clear/encrypted text\n\n" \
	"  --help  display this help and exit\n"

// Rotation de 7 bits
static t_u32 rot7(t_u32 n)
{
	return (n << 7 | n >> 25);
}

// k0 = x1L xor ((x0L xor x0R) <<< 7)
static t_u32 k0(t_u64 x0, t_u64 x1)
{
	return ((x1 >> 32) ^ rot7((x0 >> 32) ^ (x0 & 0xffffffff)));
}

// k1 = x1R xor ((x1L xor x0R) <<< 7)
static t_u32 k1(t_u64 x0, t_u64 x1)
{
	return ((x1 & 0xffffffff) ^ rot7((x1 >> 32) ^ (x0 & 0xffffffff)));
}

int main(int ac, char **av)
{
	if (check_help(ac, av, USAGE, DESC))
		return (0);
	if (ac != 3)
	{
		errno = EINVAL;
		return print_error(av[0]);
	}
	t_u64 x0, x1;

	x0 = parse(av[1], ULONG_MAX);
	x1 = parse(av[2], ULONG_MAX);
	if (errno)
		return print_error(av[0]);
	printf("k0 = 0x%08x\n", k0(x0, x1));
	printf("k1 = 0x%08x\n", k1(x0, x1));
	return (0);
}
