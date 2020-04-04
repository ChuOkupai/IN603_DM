#pragma once
#include "type.h"

typedef struct s_generator
{
	t_u8	F; // Le masque de permutations
	t_u16	L[3]; // Les 3 LFSRs
}	t_generator;

/*
** Initialise le générateur à partir des arguments de l'utilisateur.
** av[0] contient F, av[1] la clé K et av[2] le nombre de bits à afficher
** Retourne le nombre de bits à afficher.
** En cas d'erreur, errno est différent de 0.
*/
t_u64 generator_init(char **arg, int size, t_generator *g);

/*
** Génère n bits à partir du générateur.
** Si debug est différent de 0, affiche le système à chaque itération.
*/
void generator_run(t_generator *g, t_u64 n, int debug);

/*
** Effectue une attaque sur le générateur.
** La fonction de filtrage est définie par défaut.
*/
t_generator generator_attack(const char *s, t_u64 len);
