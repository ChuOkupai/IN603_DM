# IN603 - DM

## Compilation

Les fichiers sources se trouvent dans le répertoire src.
Un Makefile permet de compiler les sources.
Le compilateur utilisé est gcc, il est modifiable dans la variable CC.

Commande de compilation:
	$> make

## Utilisation

Commande pour tester les binaires:
	$> make test

La commande make rand-geffe génère un test aléatoire.
Exemple:
	$> make rand-geffe

Il est possible de lancer chaque binaire à partir du répertoire bin.
Exemple:
	$> ./bin/geffe 0b101010 0xff00eea30011 16

### Options

Chaque binaire possède une option --help pour afficher l'aide.
Exemple:
	$> ./bin/geffe --help

Le générateur de type Geffe peut être lancé en précisant l'option '--debug'.
Cette option permet d'afficher l'état du système à chaque itération.
Cependant cette fonctionnalité nécessite un terminal qui supporte l'unicode.

Exemple:
	$> ./bin/geffe --debug 0b01011011 0x7e2be09a3261 1
	┌─────────┬───┬───┬───┬───┬───┬───┬───┬───┐
	│x0x1x2   │000│001│010│011│100│101│110│111│
	├─────────┼───┼───┼───┼───┼───┼───┼───┼───┤
	│F(x0x1x2)│0  │1  │0  │1  │1  │0  │1  │1  │
	└─────────┴───┴───┴───┴───┴───┴───┴───┴───┘

	L0 ┌0111111000101011──┐
	   └────────┴──┴──┴┘  │
	L1 ┌1110000010011010─┐│
	   └────┴───┴─────┴┘ ││
	L2 ┌0011001001100001┐││
	   └──────────┴─┴┴─┘│││
	                  F(101) = 0
