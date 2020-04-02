#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_help(int ac, char **av, void (*print_help)(char *bin))
{
	for (int i = 1; i < ac; ++i)
		if (!strcmp(av[i], "--help")) // Affichage de l'aide
		{
			print_help(basename(av[0]));
			return (1);
		}
	return (0);
}

int print_error(char *bin)
{
	fprintf(stderr, "error: %s\nTry '%s --help' for more information.\n",
	strerror(errno), basename(bin));
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

unsigned long parse(const char *s, unsigned long max)
{
	unsigned long	base = 10;
	unsigned long	c;
	unsigned long	n = 0;

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
