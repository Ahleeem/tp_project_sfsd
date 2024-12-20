/***************************may*************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define b 512 //taille du bloc
#define MaxIndex 100

typedef struct {
    char tab[b];//tableau de caracteres
    int Suiv;//num du bloc suivant
} Bloc;

typedef struct Bloc Buffer;  // declaration du tampon

typedef struct Entete        // structure de l'entete du fichier en mémoire centrale
{
    int nbbloc;
    int tete;
    int queue;
    int indice_libre;   // l'indice libre dans le bloc de la queue
    int nb_car_sup;

}Entete;

typedef struct {
    FILE *file;
    Entete entete;

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


Fichier ouvrir(char *nom, char *mode) {
    Fichier f;
    f.file = fopen(nom, mode);

    f.entete.nbbloc = 0;
    f.entete.tete = 0;
    f.entete.queue = 0;
    f.entete.indice_libre = 0;
    f.entete.nb_car_sup = 0;
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

void construireIndex(Fichier *f, Tcouple *Index, int *taille) {
    int i = 1, j = 1;
    Bloc buf;
    lireBloc(f, i, &buf);
    while (i < f->entete.nbbloc || j != f->entete.indice_libre) {
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
        while (*i < F.entete.nbbloc || *j != F.entete.indice_libre) {
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
        F.entete.nb_car_sup += atoi(ch) + 4;//update de l'entete

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
//Fonction de recherche sans utiliser l'index
void recherche(char nomfichier[], char cle[20], int *trouv, int *i, int *j ) {
    Fichier F = ouvrir(nomfichier, "r");
    Bloc buf;
    Entete entete; // Utilisez la nouvelle déclaration de l'En-Tête ici
    *i = 1;
    lireBloc(&F, *i, &buf);
    while ( *i < entete.nbbloc || *j != entete.indice_libre ) { // i<denierBloc OU j<>posLibre
        // a chaque itération, on traite un enreg
        char chLong[3];
        recuperer_chaine(&F, 3, *i, j, chLong, &buf ); // récupère longueur de l'enreg
        char eff[1];
        recuperer_chaine(&F, 1, *i, j, eff, &buf ); // le car d'effacement
        char chCle[20];
        recuperer_chaine(&F, 20, *i, j, chCle, &buf ); // et la clé de l'enreg
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




/**********************ahlem********************************************/
//******************allouer et liberer********************************
void  alloc_bloc(Fichier *fichier)
{

        Bloc *buf=malloc(sizeof(Bloc));        // allocation du Buffer
        lireBloc(fichier,entete(fichier,3),buf);    // lecture du bloc correspondant a la queue
        buf->Suiv=entete(fichier,1)+1;          // mise a jour du suivant de la queue au bloc correspondant a la nouvelle queue
        ecrireBloc(fichier,entete(fichier,3),buf);  // ecriture du bloc de queue dans le fichier
        aff_entete(fichier,3,entete(fichier,1)+1);// mise a jour du numero du bloc correspondant a la nouvelle queue dans l'entete
        buf->Suiv=-1;                          // mise a jour du suivant a nill
        sprintf(buf->tab,"%s","");                // vider la chaine du buffer  apres convertion
        ecrireBloc(fichier,entete(fichier,3),buf); // ecriture du buffer dans le bloc representatnt la nouvelle queue
        aff_entete(fichier,1,entete(fichier,1)+1); // incrémentation du nombre de bloc alloués
}


/*
void libererBloc(Fichier *f, int i) {
    Bloc buffer;
    lireBloc(f, i, &buffer);    // Lire le bloc à libérer
    free(buffer.tab);           // Libérer la mémoire allouée pour le tableau
    buffer.nb = entete(f, 1);    // Le bloc suivant est l'ancien premier bloc libre
    ecrireBloc(f, i, &buffer);   // Écrire le bloc libéré dans le fichier
    affecterEntete(f, 1, i);      // Le nouveau premier bloc libre est le bloc libéré
}
*/

void creation_fichier(Fichier *fichier,int n)
{
    int k=0, cle;
    char *info=malloc(sizeof(char)*50);
    aff_entete(fichier,1,1);     //nbr totale de bloc allouer =1

    for(k=0;k<n;k++)
    {
        printf("\t veuillez introduire la cle que vous voulez inserer\n");
        scanf("%d",&cle);        // la cle a inserer
        printf("\t veuillez introduire l'info que vous voulez inserer\n\n");
        scanf("%s",info);        //info a inserer
        if(cle_correct(cle))
           {

            insertion(fichier,cle,info);
           }
    }
}

void insertion(Fichier *fichier, int cle, char *info)
{
    int i,j,trouv=0,cpt;
    Bloc buf;                                     //Une structure qui représente un tampon ou un bloc de données.
     //Alloue de la mémoire pour une chaîne de caractères qui sera utilisée pour stocker l'enregistrement à insérer
    char *cle_ch=malloc((sizeof(char))*100);            // la chaine qui va contenir l'enregistrement qui va etre inseré

    recherche(fichier,cle,&trouv,&i,&j);               // recherche de la cle pour eviter les  doublon
    if(entete(fichier,1)==0)                           // si la cle n'a  pas été trouvé alors on insere dans le premier bloc
      aff_entete(fichier,1,1);      // Si l'entête indique que le fichier est vide, alors on initialise l'entête
    if(trouv==0)
    {
        cpt=0;
        i=entete(fichier,3);                             // positionnement dans le queue
        j=entete(fichier,4);                            // positionnnement a la position libre dans le bloc de queue
        lireBloc(fichier,i,&buf);                       // lecture du  bloc actuel
        sprintf(cle_ch,"%s","");                      //convertir en chaine
        concat(cle_ch,cle,info);                     //Concaténation de la clé et de l'information pour former l'enregistrement.
                                                    // creation de l'enregistrement
        ecrire_chaine(fichier,strlen(cle_ch),&i,&j,cle_ch,&cpt,&buf);   // ecriture de la chaine construite dans le bloc actuel

        //ecriredir(fichier,i,&buf);                                    // ecriture du dernier bloc
        aff_entete(fichier,3,i);                                        // mise a jour de la quee
        aff_entete(fichier,4,j);                                        // mise a jour de la position libre
        // Mise à jour des entêtes pour refléter la nouvelle position dans la queue et la nouvelle position libre dans le bloc.
    }
    else
    {
        printf(" la cle existe deja\n");
    }
}





/**********affecter entete*****************************/
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
            fichier->entete.queue=valeur;  // numero du bloc representatnt la tete duf icheir
        }break;
         case 4:
        {
            fichier->entete.indice_libre=valeur; // la postion libre dans le bloc de queue
        }break;
         case 5:
        {
            fichier->entete.nb_car_sup=valeur;  // nombre de caractères suprimé depuis la
                                                  //création du ficher afin de lancer la réorganiosation
        }break;

    };
}


/******************entete**************************/
//permet d'obtenir les elements de l'entete du ficher
int entete(Fichier *fichier, int i)
{

    switch(i)
    {
        case 1:
        {
            return(fichier->entete.nbbloc); // nb bloc allouer
        }break;
         case 2:
        {
            return(fichier->entete.tete); // tete
        }break;
         case 3:
        {
            return(fichier->entete.queue); // la queue
        }break;
         case 4:
        {
            return(fichier->entete.indice_libre); // indice libre
        }break;
         case 5:
        {
            return(fichier->entete.nb_car_sup); // nb carac supremer //A confimer si on a besoin
        }break;

    };
}



/********************ecrirechaine*********************/
void ecrire_chaine(Fichier *fichier,int n , int *i, int *j, char chaine[],int *cpt,Bloc *buf)
{

    int k=0;
    (*cpt)=0;                         // nombre de bloc ajoutés
    for(k=0;k<n;k++)                // k pour le deplacement dans la chaine
    {
        if((*j)<=98)                //si je suis toujours dans le bloc conserné
        {
            buf->tab[*j]=chaine[k]; // affectation des caractères de la chaine dans le buffer un a un
            (*j)++;                  // deplacement dans le buffer
        }
        else                          // si la chaine a inserer depasse le buffer
         {
            ecrireBloc(fichier,*i,buf);  // ecriture du precedent buffer dans le fichier
            alloc_bloc(fichier);        // allocation d'un nouveau bloc afin de recevoir le reste de la chaine
            buf->tab[0]=chaine[k];      // ecrtiture du k°eme caractère de la chaine dans la position 0
            (*j)=1;                     // passage a la position 1
            (*i)=entete(fichier,3);     // deplacement dans les bloc du ficher
           (*cpt)++;                   // incrementation du nombre de bloc alloues
        }
    }
    buf->tab[*j]='\0';                  // fin de la chaine

}


//----------------------------------- procedure qui transforme un nombre en chaine de caract�re sur longueur de caract�re-------------//
void turn_to_string(char chaine[], int n, int longueur)
{

    int k;
    for(k=longueur-1;k>=0;k--)          // on commence par le poids faible du nombre qui va etre mi a chaine[longueur-1]
    {
        chaine[k]=(n%10)+'0';           // extraction chiffre par chiffre  grace au mod par 10 et ajout du code ascii du zero afoin d'obtenir le code ascii correspondant au chiffre obtenu
        n=n/10;                        // on passe au chiffre suivant
    }
    chaine[longueur]='\0';             // fin de la chaine construite
}


/*************************conctat**************************/
//-fonction qui permet de construire la chaine correspondant a la forme de l'enregistrememnt//
void concat(char chaine[], int cle, char info[])  //  a inserer dans le ficheir a partir de la cle et de l'info
{

    char ch_f[100];
    turn_to_string(ch_f,cle,5);                   // transformation de la cle en chaine sur 5 positions
    strcat(ch_f,info);                            // concaténation de cle et info
    turn_to_string(chaine,strlen(info),3);        // construction du debut de la chaine finale en commençant par la taille de l'info
    strcat(chaine,"f");                           // mise a jour du champs effacé
    strcat(chaine,ch_f);     }                     // construction de la chaine finale avec l'ordre suivant taille efface cle info



//----------------------------------affichage du bloc----------------------------------------------------
void afficher_bloc(Fichier *fichier,int i)
{
    Bloc buf;
    lireBloc(fichier,i,&buf); //lecture du ieme bloc
    printf("le tableau de caractères de bloc %d contient\n\n",i);
    printf("%s\n",buf.tab); //affichage de la chaine de carctères contenue
    printf(" le suivant du bloc %d  est %d\n",i,buf.Suiv); // affichage du suivant

}

//------------------------ affichage du ficher----------------------------------------------------------------//
void afficher_fichier(Fichier *fichier)
{
    int i=entete(fichier,2); // positionnement a la tete du fichier
    int j=0;                  // positionnement au premier caractère
    int stop=0;
    Bloc buf;
    char *ch1=malloc(sizeof(char)*3);
    char *ch2=malloc(sizeof(char));
    char *ch3=malloc(sizeof(char)*5);
    char *ch4=malloc(sizeof(char)*100);
    lireBloc(fichier,i,&buf);    // lecture du premier bloc
  if(entete(fichier,1)!=0)
  {
    while(!stop)
    {
        recuperer_chaine(fichier,3,&i,&j,ch1,&buf); // on commence a recuperer les enregistrememnt un a un
        recuperer_chaine(fichier,1,&i,&j,ch2,&buf);
        recuperer_chaine(fichier,5,&i,&j,ch3,&buf);
        recuperer_chaine(fichier,atoi(ch1),&i,&j,ch4,&buf);
        if(strcmp(ch2,"f")==0) //. si l'info n' pas ete supprimé logiquement
        {
            printf("%s%s%s%s",ch1,ch2,ch3,ch4); //impression de la totalité de l'enregistrement

        }
        if((i==entete(fichier,3)) && (j==entete(fichier,4))) // si on est la fin du ficher on arrete
        {
            stop=1;
        }
        system("pause");
    }
  }
  else
  {
      printf("fichier vide");
  }
}


void afficher_entete(Fichier *fichier) // affichage de l'entete
{
    printf("\nles ceratctéristiques du fiocher sont les suivantes\n--------------------------------------------");
    printf("le nombre total de bloc alloué est   %d\n",entete(fichier,1));
    printf("l'entete du ficher se trouve au bloc    %d\n",entete(fichier,2));
    printf("la queue du ficher se trouve au bloc    %d\n",entete(fichier,3));
    printf("la postion libre dans le bloc de queue est    %d\n",entete(fichier,4));
    printf("le nombre total de caractère supprimé est    %d\n",entete(fichier,5));
}


//**************************fonction de verification de la cle*************//
int nb_pos(int a)
{
    int cpt=1;
    while((a/10)!=0)
    {
        a=a/10 ;
        cpt++ ;
    }
   return(cpt);
}



int cle_correct(int cle)
{
  int  correct=0 ;
  if(nb_pos(cle)>5)

  {
      puts("cle n'est pas correct") ;
  }
  return(correct) ;

}












//***************************inas*************************************
/* on ouvre le fichier2 pour stocker les enregistrement non suprimè physiquement
dans la suppression on intialise pose1 pose2 bloc1 bloc2 qui represente la position dans les blocs de deuq fichiers
on lit dans le buf1 le 1er bloc de fichier original on utilison lireBloc 
on parcour les les blocs du fichier et on recupere les enregistrement un a un
si un enregistrement n'est pas logiquement suprime elle l'ecrit dans le bnouveau fichier
*/
/**************suppression phy************************/
void suppression_physique_L7OV7C(Fichier *fichier, char *nom_physique)
{
    Bloc buf1, buf2;
    Fichier *fichier2;
    int bloc1, bloc2, pos1, pos2, cpt = 0;
    char *ch1 = malloc(sizeof(char) * 3);
    char *ch2 = malloc(sizeof(char));
    char *ch3 = malloc(sizeof(char) * 5);
    char *ch4 = malloc(sizeof(char) * 50);
    char *chaine = malloc(sizeof(char) * 100);
    *fichier2 = ouvrir(nom_physique, 'n'); // ouverture du fichier
    bloc1 = entete(fichier, 2);
    pos1 = 0;
    aff_entete(fichier2, 1, 1);
    bloc2 = 1;
    pos2 = 0;
    lireBloc(fichier, bloc1, &buf1); // lecture du premier bloc dans le fichier 1
    int stop = 0;

    while (bloc1 != 0 && pos1 != -1)
    {
        sprintf(chaine, "%s", ""); // recuperation des chaines
        recuperer_chaine(fichier, 3, &bloc1, &pos1, ch1, &buf1);
        recuperer_chaine(fichier, 1, &bloc1, &pos1, ch2, &buf1);
        recuperer_chaine(fichier, 5, &bloc1, &pos1, ch3, &buf1);
        recuperer_chaine(fichier, atoi(ch1), &bloc1, &pos1, ch4, &buf1);
        strcat(chaine, ch1); // concatenation des chaines obtenues
        strcat(chaine, ch2);
        strcat(chaine, ch3);
        strcat(chaine, ch4);

        if (strcmp(ch2, "f") == 0) // si la cle n'a pas ete supprime logiquement on l'insere dans le nouveau fichier
        {
            ecrire_chaine(fichier2, strlen(chaine), &bloc2, &pos2, chaine, &cpt, &buf2);
            aff_entete(fichier2, 3, bloc2);
            aff_entete(fichier2, 4, pos2);
        }

        if ((bloc1 == entete(fichier, 3)) && (pos1 == entete(fichier, 4))) // si on est a la fin du fichier source on arret
        {
            break;
        }

        lireBloc(fichier, bloc1, &buf1); // lecture du bloc suivant
    }

    ecrireBloc(fichier2, bloc2, &buf2);
    fermer(fichier2);
}
/**************fermeture****************/
/* on repositionne le curseur au debut du fichier pour garantir que le prochain acces au fichier se seran
depuis le debut 
on lit le contenu du 1er bloc de fichier dans le buf_temp (elle permet de s'assurer que le premeir bloc du fichier est a jour apres la 
reouverture de fichier)
on ferme le ficher avec fclose
*/
void fermer(Fichier *fichier)
{
    // Repositionnement du curseur en debut de fichier
    rewind(fichier->file);

    // Utilisation d'un tampon temporaire pour eviter le chevauchement
    Bloc buf_temp;
    lireBloc(fichier, 0, &buf_temp);
    // Reecriture du tampon temporaire dans le fichier
    ecrireBloc(fichier, 0, &buf_temp);

    // Fermeture du fichier
    fclose(fichier->file);
}
/* elle utilise la fonction fseek pour deplacer le curseur de lecture dans le fichier a la position 
du debut du bloc specifie par l'indice i
    elle utilise fread pour lire les donne du bloc depuis le fichier vers la tampon buf
elle renetialise la position du curseur dans le fichier en utilisant la fonction rewind 
*/
void lireBloc(Fichier *fichier, int i, Bloc *buf)
{

    Bloc buf_temp;


    fseek(fichier->file,(sizeof(Entete)+sizeof(Bloc)) * (i - 1), SEEK_SET);
   fread(buf,sizeof(Bloc),1,fichier->file);
       rewind(fichier->file);

}
int main() {
    Fichier fichier = ouvrir("C:\\Users\\Thinkpad\\Desktop\\maymehenni\\fichier_test.dat", "r");

    insertion(&fichier, 123, "Donnee123");
    insertion(&fichier, 456, "Donnee456");
    insertion(&fichier, 789, "Donnee789");

    printf("Contenu du fichier avant suppression logique :\n");
    afficher_fichier(&fichier);

    printf("\nSuppression logique en cours :\n");
    Sup("Donnee456", "fichier_test.dat");

    printf("\nContenu du fichier après suppression logique :\n");
    afficher_fichier(&fichier);

    printf("\nSuppression physique en cours :\n");
    suppression_physique_L7OV7C(&fichier, "fichier_temp.dat");

    printf("\nContenu du fichier après suppression physique :\n");
    afficher_fichier(&fichier);

    printf("\nCréation de fichier en cours :\n");
    creation_fichier(&fichier, 2);

    printf("\nContenu du fichier après création :\n");
    afficher_fichier(&fichier);

    printf("\nOpérations de recherche en cours :\n");
   int trouve = 0, i = 0, j = 0, ind = 0;
    Recherche_Liste_Variable_NonOrdonnee("fichier_test.dat", "Donnee123", Index, MaxIndex, &trouve, &ind, &i, &j);
  printf("Résultat de la recherche pour Donnee123 : %s\n", trouve ? "Trouvé" : "Non Trouvé");

    trouve = 0;
    recherche("fichier_test.dat", "Donnee789", &trouve, &i, &j);
    printf("Résultat de la recherche pour Donnee789 : %s\n", trouve ? "Trouvé" : "Non Trouvé");

    printf("\nSuppression d'index en cours :\n");
    Sup("Donnee123", "fichier_test.dat");

    printf("\nContenu du fichier après suppression d'index :\n");
    afficher_fichier(&fichier);

    fermer(&fichier);

    return 0;
}
