#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define b 512 //taille du bloc
#define MaxIndex 100

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
//declaration de l'index
typedef struct maillon {
    struct {
        int numblc;
        int depl;
    } val;

struct maillon *adr;
} Maillon;

typedef struct {
    char cle[20];
    Maillon *tete;
} Tcouple;

Tcouple Index[MaxIndex];
Fichier ouvrir(char *nom, char *mode) {
Fichier f;
f.file = fopen(nom, mode);
    if (f.file == NULL) {
        perror("erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
     for (int i = 0; i < 4; i++) {
        f.entete[i] = 0;
    }
    return f;


}
void ecrireBloc(Fichier *f, int i, Bloc *buffer) {
    if (f->file != NULL) {
        fseek(f->file, i * sizeof(Bloc), SEEK_SET);
        size_t result = fwrite(buffer, sizeof(Bloc), 1, f->file);
        if (result != 1) {
            perror("erreur d'ecriture du bloc");
            exit(EXIT_FAILURE);
        }
    }
}

void recuperer_chaine( int n, int i, int *j, char *ch, Bloc *buf ) {
    int k;
    for (k = 0; k < n; k++) {
        ch[k] = buf->tab[*j];
        (*j)++;
        if (*j > b) {
            *j = 1;
            i = buf->Suiv;
            lireBloc(&F, i, buf);
        }
    }
    ch[k] = '\0';
}

void construireIndex(Fichier *f, Tcouple *Index) {
    int i = 1, j = 1, ind = 0;
    Bloc buf;
    lireBloc(f, i, &buf);
    while (i < entete(f, 1) || j != entete(f, 2)) {
        char chLong[3];
        recuperer_chaine(3, i, &j, chLong, &buf);
        recuperer_chaine(1, i, &j, eff, &buf);
        char chCle[20];
        recuperer_chaine(20, i, &j, chCle, &buf);
        if (eff[0] == 'N') {

        strcpy(Index[ind].cle, chCle);
    Index[ind].tete = malloc(sizeof(Maillon));
    Index[ind].tete->val.numblc = i;
    Index[ind].tete->val.depl = j - 20;
    Index[ind].tete->adr = NULL;
            ind++;
        }

        j = j + atoi(chLong);
        if (j > b) {j = 1;
            i = buf.Suiv;
            lireBloc(f, i, &buf);
        }
    }
}
void Sup(char *cle, char *nomfichier) {
    int trouv, i, j, ind;
    Recherche_Liste_Variable_NonOrdonnee(nomfichier, cle, Index, MaxIndex, &trouv, &ind, &i, &j);
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
