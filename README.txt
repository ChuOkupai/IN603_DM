# IN603 - DM

## Compilation

Les fichiers sources se trouvent dans le répertoire src.
Un Makefile permet de compiler les sources.
Le compilateur utilisé est gcc, il est modifiable dans la variable CC.

Commande de compilation:
$> make
OU
$> make all

## Utilisation

Commande pour tester les binaires:
$> make test

Il est possible de lancer chaque binaire à partir du répertoire bin.
Exemple:
$> ./bin/geffe 0b101010 0xff00eea30011 16

Chaque binaire possède une option --help pour afficher l'aide.
Exemple:
$> ./bin/geffe --help
