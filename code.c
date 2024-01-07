#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define b 512 //taille du bloc

/*les caracteristiques sont:
1- Numéro du premier bloc .
2-Numéro du dernier bloc.
3-La 1ère position libre dans le dernier bloc.
4-Le nombre de caractères perdus suite aux suppressions logiques.*/
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
    int k;
    for (k = 0; k < n; k++) {
        ch[k] = buf->tab[*j];
        (*j)++;
        if (*j > b) {
            *j = 1;
            i = buf->Suiv;
            lireBloc(file, i, buf);
        }
    }
    ch[k] = '\0'; // Ajout du caractère de fin de chaîne
}


}
Recherche séquentielle
void Rech( char cle[20], char nomfichier[], int *trouv, int *i, int *j ) {
    Fichier F = ouvrir(nomfichier, "r");
    Bloc buf;
    *i = 1;
    lireBloc(&F, *i, &buf);
    while ( *i < entete(&F,1) || *j != entete(&F,2) ) { // i<denierBloc OU j<>posLibre
        // a chaque itération, on traite un enreg
        char chLong[3];
        recuperer_chaine( 3, *i, j, chLong, &buf ); // récupère longueur de l'enreg
        char eff[1];
        recuperer_chaine( 1, *i, j, eff, &buf ); // le car d'effacement
        char chCle[20];
        recuperer_chaine( 20, *i, j, chCle, &buf ); // et la clé de l'enreg
        if ( strcmp(chCle, cle) == 0 && eff[0] == 'N' )
            *trouv = 1;
        else {
            // on passe au suivant
            *j = *j + atoi(chLong); // on a deja lu les 20 car de la cle
            if ( *j > b ) {
                *j = 1;
                *i = buf.Suiv;
                lireBloc(&F, *i, &buf);
            }
        }
    }
    fermer(&F);
}
Suppression logique
void Sup(char *cle, char *nomfichier) {
    int trouv, i, j;
    Rech(cle, nomfichier, &trouv, &i, &j);
    if (trouv) {
        Fichier F = ouvrir(nomfichier, "a");
        Bloc buf;
        lireBloc(&F, i, &buf);
        char ch[3];
        recuperer_chaine(3, i, &j, ch, &buf);
        if (j <= b) {
            buf.tab[j] = 'E';
        } else {
            i = buf.Suiv;
            lireBloc(&F, i, &buf);
            buf.tab[1] = 'E';
        }
        ecrireBloc(&F, i, &buf);
        affecterEntete(&F, 4, entete(&F, 4) + atoi(ch) + 4);
        fermer(&F);
    }
}

