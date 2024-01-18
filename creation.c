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


//*****************************my declaration
/*
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
*/


void creation_fichier(Fichier *fichier,int n)
{
    int k=0, cle;
    char *info=malloc(sizeof(char)*50);
    aff_entete(fichier,1,1); //nbr totale de bloc allouer =1

    for(k=0;k<n;k++)
    {
        printf("\tveuillez introduire la cle que vous voulez inserer\n");
        scanf("%d",&cle);     // la cle a inserer
        printf("\t veuillez introduire l'info que vous voulez inserer\n\n");
        scanf("%s",info);     //info a inserer
        if(cle_correct(cle))
           {

            insertion(fichier,cle,info);
           }
    }
}

//permet de mettre a jour les elements de l'entete du ficher
void aff_entete(Fichier *fichier,int i , int valeur)
{

    switch(i)
    {
        case 1:
        {
            fichier->entete.nbbloc=valeur; // nombre total de bloc alloués dans le fichier
        }break;
         case 2:
        {
            fichier->entete.tete=valeur;  // numero du bloc representatnt la tete du fichier
        }break;
         case 3:
        {
            fichier->entete.queue=valeur;  // numero du bloc representatnt la tete du fichier
        }break;
         case 4:
        {
            fichier->entete.indice_libre=valeur; // la postion libre dans le bloc de queue
        }break;
         case 5:
        {
            fichier->entete.nb_car_sup=valeur;  // nombre de caractères suprimé depuis la
                                                  //création du ficher 
        }break;

    };
}










/*

// ********Fonction pour ajouter un bloc avec des données
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
// ************Fonction pour afficher les blocs
void printBloc(Bloc *head) {
    Bloc *current = head;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}
*/
