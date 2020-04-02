#pragma once
#include <stdint.h>

// Affiche plus d'informations si != 0
#define DEBUG 0

typedef struct s_generator
{
	uint8_t		F; // Le masque de permutations
	uint16_t	L[3]; // Les 3 LFSRs
}	t_generator;

/*
** Initialise le générateur à partir des arguments de l'utilisateur
** av[0] contient F, av[1] la clé K et av[2] le nombre de bits à afficher
** Retourne le nombre de bits à afficher
** En cas d'erreur, errno est différent de 0.
*/
uint64_t generator_init(int ac, char **av, t_generator *g);

// Génère n bits à partir du générateur
void generator_run(t_generator *g, uint64_t n);
