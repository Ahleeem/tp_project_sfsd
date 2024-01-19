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
//l'index est un index dense de type liste chainee
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

//implementation de la fonction ouvrir
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

//implementation de la fonction ecrire
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

//Implementation d'un fonction necessaire dans la fonction de recherche
void recuperer_chaine(Fichier *f, int n, int i, int *j, char *ch, Bloc *buf ) {
    int k;
    for (k = 0; k < n; k++) {
        ch[k] = buf->tab[*j];
        (*j)++;
        if (*j > b) {
            *j = 1;
            i = buf->Suiv;
            lireBloc(f, i, buf);
        }
    }
    ch[k] = '\0';
}

//ajout  d'une var taille pour pouvoire rajouter des elements a l'index en incrementant la taille
void construireIndex(Fichier *f, Tcouple *Index, int *taille) {
    int i = 1, j = 1;
    Bloc buf;
    lireBloc(f, i, &buf);
    while (i < entete(f, 1) || j != entete(f, 2)) {
        char chLong[3];
        recuperer_chaine(f, 3, i, &j, chLong, &buf);
        char eff[1];
        recuperer_chaine(f, 1, i, &j, eff, &buf);
        char chCle[20];
        recuperer_chaine(f, 20, i, &j, chCle, &buf);
        if (eff[0] == 'N') {
        strcpy(Index[*taille].cle, chCle);
        Index[*taille].tete = malloc(sizeof(Maillon));
        Index[*taille].tete->val.numblc = i;
        Index[*taille].tete->val.depl=j - 20;
        Index[*taille].tete->adr = NULL;
            (*taille)++;
        }
        j = j+atoi(chLong);
        if (j>b) {
            j= 1;
            i= buf.Suiv;
            lireBloc(f, i, &buf);
        }
    }
}

//code a ajouter dans la fonction d'insertion pour mettre a joure l'index apres chaques insertion
/*Ajout dans l'entete de la fonction du parametre index et modification du corp
void ajouterValeur(Fichier *f, char *cle, Tcouple *Index, int *taille)
    strcpy(Index[*taille].cle, cle);
    Index[*taille].tete = malloc(sizeof(Maillon));
    Index[*taille].tete->val.numblc =// num bloc de la valeur inseree;
    Index[*taille].tete->val.depl = ;
    Index[*taille].tete->adr = NULL;
    (*taille)++;*/

void Recherche_Liste_Variable_NonOrdonnee(char *nom_f, char *cle, Tcouple *Index, int taille, int *trouv, int *ind, int *i, int *j) {
    Fichier F = ouvrir(nom_f, "r");
    Bloc buf;
    *i = 1;
    lireBloc(&F, *i, &buf);
    //recherche dans l'index
    *ind = 0;
    while (*ind < taille && strcmp(Index[*ind].cle, cle) < 0) {
        (*ind)++;
    }
    if (*ind < taille && strcmp(Index[*ind].cle, cle) == 0) {
        //si la cle est dans l'index, aller directement au bloc correspondant.
        *i = Index[*ind].tete->val.numblc;
        *j = Index[*ind].tete->val.depl;
        lireBloc(&F, *i, &buf);
        *trouv = 1;
    } else {
        *trouv = 0;
        char eff[1]; // Ajout de la déclaration de eff
        while (*i < entete(&F,1) || *j != entete(&F,2)) {
            char chLong[3];
            recuperer_chaine(&F, 3, *i, j, chLong, &buf);
            recuperer_chaine(&F, 1, *i, j, eff, &buf);
            char chCle[20];
            recuperer_chaine(&F, 20, *i, j, chCle, &buf);
            if (strcmp(chCle, cle) == 0 && eff[0] == 'N') {
                *trouv =1;
            } else {
                *j = *j + atoi(chLong);
                if (*j > b) {
                    *j = 1;
                    *i = buf.Suiv;
                    lireBloc(&F, *i, &buf);
                }
            }
        }
    }
    fermer(&F);
}


 //implementation de la focntion de suppression logique
 //N veut dire non efface et E veut dire efface
void Sup(char *cle, char *nomfichier) {
    int trouv, i, j, ind;
    Recherche_Liste_Variable_NonOrdonnee(nomfichier, cle, Index, MaxIndex, &trouv, &ind, &i, &j);
    if (trouv) {
        Fichier F = ouvrir(nomfichier, "a");
        Bloc buf;
        lireBloc(&F, i, &buf);
        char ch[3];
        recuperer_chaine(&F, 3, i, &j, ch, &buf);
        if (j <= b) {
            buf.tab[j] = 'E';
        } else {
            i = buf.Suiv;
            lireBloc(&F, i, &buf);
            buf.tab[1] = 'E';
        }
        ecrireBloc(&F, i, &buf);
        affecterEntete(&F, 4, entete(&F, 4) + atoi(ch) + 4);//update de l'entete

        //Mise a jour de l'index apres la suppression
    Maillon *courant = Index[ind].tete;
    Maillon *precedent = NULL;
        while (courant != NULL) {
            if (courant->val.numblc == i && courant->val.depl == j - 20) {
                if (precedent == NULL) {
                    Index[ind].tete = courant->adr;
                } else {
                    precedent->adr = courant->adr;
                }
                free(courant);
                break;
            }
            precedent = courant;
            courant = courant->adr;
        }

        fermer(&F);
}
}
void  alloc_bloc(Fichier *fichier)
{

        Buffer *buf=malloc(sizeof(Buffer));        // allocation du Buffer
        liredir(fichier,entete(fichier,3),buf);    // lecture du bloc correspondant a la queue
        buf->suivant=entete(fichier,1)+1;          // mise a jour du suivant de la queue au bloc correspondant a la nouvelle queue
        ecriredir(fichier,entete(fichier,3),buf);  // ecriture du bloc de queue dans le fichier
        aff_entete(fichier,3,entete(fichier,1)+1);// mise a jour du numero du bloc correspondant a la nouvelle queue dans l'entete
        buf->suivant=-1;                          // mise a jour du suivant a nill
        sprintf(buf->tab,"%s","");                // vider la chaine du buffer  apres convertion
        ecriredir(fichier,entete(fichier,3),buf); // ecriture du buffer dans le bloc representatnt la nouvelle queue
        aff_entete(fichier,1,entete(fichier,1)+1); // incrémentation du nombre de bloc alloués
}



void libererBloc(Fichier *f, int i) {
    Bloc buffer;
    lireBloc(f, i, &buffer);    // Lire le bloc à libérer
    free(buffer.tab);           // Libérer la mémoire allouée pour le tableau
    buffer.nb = entete(f, 1);    // Le bloc suivant est l'ancien premier bloc libre
    ecrireBloc(f, i, &buffer);   // Écrire le bloc libéré dans le fichier
    affecterEntete(f, 1, i);      // Le nouveau premier bloc libre est le bloc libéré
}


