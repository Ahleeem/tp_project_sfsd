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
Fichier ouvrir(char *nom, char *mode) {
    Fichier f;
    f.file = fopen(nom, mode);
    if (f.file == NULL) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }


}
void Rech( char cle[20], char nomfichier[], int *trouv, int *i, int *j ) {
    Fichier F = ouvrir(nomfichier, "r");
    Bloc buf;
    *i = 1;
    lireBloc(&F, *i, &buf);
