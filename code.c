#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define b 512 //taille du bloc


typedef struct {
    char tab[b];//tableau de caracteres
    int Suiv;//num du bloc suivant
} Bloc;

typedef struct {
    FILE *file;
    int entete[4];//entete contient 4 caracteristiques

} Fichier;
