#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"

int check_help(int ac, char **av, const char *usage, const char *desc)
{
	for (int i = 1; i < ac; ++i)
		if (!strcmp(av[i], "--help")) // Affichage de l'aide
			return !!printf("Usage: %s %s\n%s", basename(av[0]), usage, desc);
	return (0);
}

int check_debug(int *ac, char **av)
{
	for (int i = 1; i < *ac; ++i)
		if (!strcmp(av[i], "--debug"))
		{
			while (++i < *ac) // L'argument est déplacé à la fin
			{
				char *s = av[i - 1];
				av[i - 1] = av[i];
				av[i] = s;
			}
			--*ac;
			return (1);
		}
	return (0);
}

int print_error(char *bin)
{
	bin = basename(bin);
	fprintf(stderr, "%s: %s\nTry '%s --help' for more information.\n",
	bin, strerror(errno), bin);
	return (EXIT_FAILURE);
}

/*
** Fonction utilitaire qui convertit un caractère en valeur hexadécimale
** Renvoie 16 si le caractère est invalide
*/
static int xtoi(int c)
{
	if (isdigit(c))
		return (c - '0');
	return ((c = tolower(c) - 'a' + 10) >= 0 && c < 16 ? c : 16);
}

t_u64 parse(const char *s, const t_u64 max)
{
	t_u64 base = 10;
	t_u64 n = 0;
	t_u64 c;

	if (*s == '0' && tolower(s[1]) == 'b')
		s += (base = 2);
	else if (*s == '0' && tolower(s[1]) == 'x')
	{
		base = 16;
		s += 2;
	}
	do
		if ((c = xtoi(*s)) >= base)
			return (errno = EINVAL);
		else if (n > (max - c) / base) // Overflow
			return (errno = ERANGE);
		else
			n = n * base + c;
	while (*(++s));
	return (n);
}

char btoa(t_u32 c)
{
	return ('0' + !!c);
}
