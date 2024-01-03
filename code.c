#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define b 512


typedef struct {
    char tab[b];
    int Suiv;
} Bloc;

typedef struct {
    FILE *file;
    int entete[4];
    BUFFER buf;
} Fichier;
