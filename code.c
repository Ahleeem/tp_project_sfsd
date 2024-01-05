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
     for (int i = 0; i < 4; i++) {
        f.entete[i] = 0;
    }
    return f;


}
void recuperer_chaine( int n, int i, int *j, char *ch, Bloc *buf ) {
    int k=0;
    for (k = 0; k < n; k++) {
        ch[k] = buf->tab[j];
        (*j)++;
        if (*j > b) {
            *j = 1
            i = buf->Suiv;
            lireBloc(&F, i, buf);
        }
    }

}
void Rech( char cle[20], char nomfichier[], int *trouv, int *i, int *j ) {
    Fichier F = ouvrir(nomfichier, "r");
    Bloc buf;
    *i = 1;
    lireBloc(&F, *i, &buf);
    while ( *i < entete(&F,1) || *j != entete(&F,2) ) {

