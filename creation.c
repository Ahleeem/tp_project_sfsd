#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max ...
/*
// Définition du type Bloc
typedef struct {
    char *tab; // Tableau de caractères contenant les données du bloc
    int ne;    // Taille du tableau tab
    int nb;    // Numéro du bloc suivant dans la liste liée
} Bloc;

typedef struct Bloc {
char data[....]; // tableau de caractères pour les enreg.
int suiv; // numéro du bloc suivant dans la liste
}Bloc;

typedef struct Bloc {
    char data[....];  // Tableau de caractères 
    struct Bloc *next;
} Bloc;
*/






typedef struct Tbloc
{
    char tab[max];  // le tableau de caractère
    int suivant;    // le numero du bloc suivant dans la liste
}Tbloc;

typedef struct Tbloc Buffer;  // declaration du tampon
typedef struct Entete        // structure de l'entete du fichier en mémoire centrale
{
    int nbbloc;
    int tete;
    int queue;
    int indice_libre;   // l'indice libre dans le bloc de la queue
    int nb_car_sup;

}Entete;

typedef struct L7OV7C
{
    FILE *fich;
    Entete entete;

}L7OV7C;

void creation_fichier(L7OV7C *fichier,int n)
{
    int k=0, cle;
    char *info=malloc(sizeof(char)*50);
    aff_entete(fichier,1,1);

    for(k=0;k<n;k++)
    {
        printf("\tveuillez introduire la cle que vous voulez inserer\n");
        scanf("%d",&cle);     // la cle a inserer
        printf("\t veuillez introduire l'info que vous voulez inserer\n\n");
        scanf("%s",info);     //info a inserer
        if(cle_correct(cle))
           {

            insertion_L7OV7C(fichier,cle,info);
           }
    }
}












/*

// Fonction pour ajouter un bloc avec des données
Bloc *addBloc(Bloc *head, const char *data) {
    Bloc *newBloc = (Block *)malloc(sizeof(Bloc));
     if (newBloc == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    strncpy(newBloc->data, data, sizeof(newBloc->data) - 1);
    newBloc->data[sizeof(newBloc->data) - 1] = '\0';  // Assure la null-termination
    newBloc->next = head;

    return newBloc;
}

*/

/*
// Fonction pour afficher les blocs
void printBloc(Bloc *head) {
    Bloc *current = head;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}
*/
