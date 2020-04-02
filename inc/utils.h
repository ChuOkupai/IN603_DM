#pragma once
#include "type.h"

/*
** Fonction utilitaire qui vérifie si l'aide est demandée par l'utilisateur.
** Si l'argument '--help' est trouvée, affiche l'aide avec usage et desc.
** Renvoie une valeur différente de 0 si l'aide à été affichée.
*/
int check_help(int ac, char **av, const char *usage, const char *desc);

/*
** Fonction utilitaire qui détecte l'option '--debug'.
** Si '--debug' est trouvée, renvoie 1 et déplace l'argument à la fin.
** Le compteur d'arguments est lui aussi modifié.
*/
int check_debug(int *ac, char **av);

/*
** Affiche une erreur à partir de la variable errno.
** Retourne toujours EXIT_FAILURE.
*/
int print_error(char *bin);

/*
** Fonction utilitaire pour récupérer l'entier à partir d'une chaîne de
** charactères.
** Détecte la base 2 si précédé de 0b ou 0B,
** la base 16 si précédé de 0x ou 0X et la base 10.
** Si l'argument contient autre chose que des caractères valides,
** errno prend la valeur EINVAL.
** Si le nombre dépasse la valeur max, errno prend la valeur ERANGE.
*/
t_u64 parse(const char *s, const t_u64 max);

// Convertit une valeur booléenne en charactère affichable
char btoa(t_u32 c);
