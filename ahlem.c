

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define b 512 //taille du bloc
#define MaxIndex 100



typedef struct
{
  char tab[b]; //tableau de carac
  int Suiv; //num du bloc suiv
} Bloc;


typedef struct Bloc Buffer;



typedef struct Entete
{
  int nbbloc;
  int tete;
  int queue;
  int indice_libre;
  int nb_car_sup;

} Entete;




typedef struct Fichier
{
  FILE *file;
  Entete entete;

} Fichier;












void aff_entete(Fichier *fichier,int i , int valeur)
{

    switch(i)
    {
        case 1:
        {
            fichier->entete.nbbloc=valeur; // nombre total de bloc alloues dans le fichier
        }break;
         case 2:
        {
            fichier->entete.tete=valeur;  // numero du bloc representant la tete du fichier
        }break;
         case 3:
        {
            fichier->entete.queue=valeur;  // numero du bloc representatnt la tete du ficheir
        }break;
         case 4:
        {
            fichier->entete.indice_libre=valeur; // la postion libre dans le bloc de queue
        }break;
         case 5:
        {
            fichier->entete.nb_car_sup=valeur;  // nombre de caracteres suprimer depuis la
                                                  //creation du ficher afin de lancer la reorganisation
        }break;

    };
}











int entete(Fichier *fichier, int i)
{

    switch(i)
    {
        case 1:
        {
            return(fichier->entete.nbbloc);
        }break;
         case 2:
        {
            return(fichier->entete.tete);
        }break;
         case 3:
        {
            return(fichier->entete.queue);
        }break;
         case 4:
        {
            return(fichier->entete.indice_libre);
        }break;
         case 5:
        {
            return(fichier->entete.nb_car_sup);
        }break;

    };
}











void liredir(Fichier *fichier, int i , Bloc *buf)
{

 fseek(fichier->file,(sizeof(Entete)+sizeof(Bloc)*(i-1)),SEEK_SET); // positionnement au debut du bloc numero i
 fread(buf,sizeof(Bloc),1,fichier->file);                         //lecture d'un bloc de caractere correspondant a la taille du bloc dans le buffer
 rewind(fichier->file);                                            // repositionnement endebut de fichier

}
void ecriredir(Fichier *fichier, int i, Bloc *buf)
{

     fseek(fichier->file,sizeof(Entete)+sizeof(Bloc)*(i-1),SEEK_SET); // positionnement au debut du bloc numero i
     fwrite(buf,sizeof(Bloc),1,fichier->file);                       //ecriture du contenu du buffer dans le bloc numero i du fichier

}











Fichier *ouvrir(char nom_physique[], char mode)
{

    Fichier *fichier=malloc(sizeof(Fichier));
    Bloc buf;
    if( (mode=='A') ||  (mode == 'a') )                         // mode ancien
    {
        fichier->file=fopen(nom_physique,"rb+");                // ouverture du fichier en mode binaire lecture et ecriture
        fread(&(fichier->entete),sizeof(Entete),1,fichier->file);  // chargement de l'entete enregistrer en debut de fichier
    }
    else
    {
        if( (mode=='N') ||  (mode == 'n') )                   // mode nouveau
        {
             fichier->file=fopen(nom_physique,"wb+");         // ouverture du fichier en mode binaire  ecriture
             aff_entete(fichier,1,0);                         // mise a zero du nombre de bloc si entete(fichier,1)=0 alors le fichier est vide
             aff_entete(fichier,2,1);                         // mettre tete au premier bloc
             aff_entete(fichier,3,1);                         // de meme pour la queue puisque au debut tete est queue sont confondues
             aff_entete(fichier,4,0);                         // le premier caract�re du ficheir correspond a laposition libre puisqu'il est nouveau
             aff_entete(fichier,5,0);                         // aucun caractere n'a encore etait supprimer
             fwrite(&(fichier->entete),sizeof(Entete),1,fichier->file);// enregistrement de l'entete dans le fichier
             buf.Suiv=-1;                                       // le suivant du premier bloc a NULL
             sprintf(buf.tab,"%s","");                       // initialisation du buffer a chaine vide
             ecriredir(fichier,1,&buf);                      // ecriture du premier bloc dans le fichier


        }
        else
        {
            printf("format d'ouverture impossible");
        }
    }
    return(fichier);

}






void fermer(Fichier *fichier)
{
    rewind(fichier->file); // repositionnement du curseur en debut de fichier
    fwrite(&(fichier->entete),sizeof(Entete),1,fichier->file); // sauvegarde de la derniere version de l'entete

    rewind(fichier->file);// repositionnement du curseur en debut de fichier
    fclose(fichier->file);// fermeture du fichier

}







void  alloc_bloc(Fichier *fichier)
{

        Bloc *buf=malloc(sizeof(Bloc));           // allocation du Buffer
        liredir(fichier,entete(fichier,3),buf);   // lecture du bloc correspondant a la queue
        buf->Suiv=entete(fichier,1)+1;           // mise a jour du suivant de la queue au bloc correspondant a la nouvelle queue
        ecriredir(fichier,entete(fichier,3),buf);// ecriture du bloc de queue dans le fichier
        aff_entete(fichier,3,entete(fichier,1)+1);// mise a jour du numero du bloc correspondant a la nouvelle queue dans l'entete
        buf->Suiv=-1;                             // mise a jour du suivant a nill
        sprintf(buf->tab,"%s","");                // vider la chaine du buffer
        ecriredir(fichier,entete(fichier,3),buf);// ecriture du buffer dans le bloc representatnt la nouvelle queue
        aff_entete(fichier,1,entete(fichier,1)+1);// incrementation du nombre de bloc allouees
}






void turn_to_string(char chaine[], int n, int longueur)
{

    int k;
    for(k=longueur-1;k>=0;k--)          // on commence par le poids faible du nombre qui va etre mit a chaine[longueur-1]
    {
        chaine[k]=(n%10)+'0';           // extraction chiffre par chiffre  grace au mod par 10 et ajout du code ascii du zero afoin d'obtenir le code ascii correspondant au chiffre obtenu
        n=n/10;                        // on passe au chiffre suivant
    }
    chaine[longueur]='\0';             // fin de la chaine construite
}








void concat(char chaine[], int cle, char info[])  //a inserer dans le ficheir a partir de la cle et de l'info
{

    char ch_f[100];
    turn_to_string(ch_f,cle,5);                   // transformation de la cle en chaine sur 5 positions
    strcat(ch_f,info);                            // concatenation de cle et info
    turn_to_string(chaine,strlen(info),3);        // construction du debut de la chaine finale en commencant par la taille de l'info
    strcat(chaine,"f");                           // mise a jour du champs efface
    strcat(chaine,ch_f);                          // constructution de la chaine finale avec l'ordre suivant taille effacee cle info
}







void recuperer_chaine(Fichier *fichier,int n , int *i, int *j, char chaine[],Bloc *buf) //recupere chaine du buffer
   {

    int k=0;
    sprintf(chaine,"%s","");
    for(k=0;k<n;k++)                   //indice de parcours de la chaine resultat
    {                                   //  j celui du tableau
        if((*j)<=98)                    // si le k imem caractere de la chaine correspondant au j ime car du buffer  est toujour dans le bloc i
        {                               // le caractere 99 correspond a la fin de la chaine dans le tableau
            chaine[k]=buf->tab[(*j)];   // recuperation du caractere dans la position k de la chaine
            (*j)++;                      // deplacement d'une position dans le buffer
        }
        else                             // si le k i eme caractere a recuperer sort du buffer
        {
            (*i)=buf->Suiv;              // on passe au suivant du bloc i dans la liste
            liredir(fichier,(*i),buf);   // lecture
            chaine[k]=buf->tab[0];       // recuperation du premier caractere du tableau de carractere correspondant au k ieme carac de la chaine
            (*j)=1;                      //positionnement eu second caractere dans le nouveau buffer
        }
    }
   chaine[k]='\0';                        // fin de la chaine obtenue
}







void ecrire_chaine(Fichier *fichier,int n , int *i, int *j, char chaine[],int *cpt,Bloc *buf)
{

    int k=0;
    (*cpt)=0;     // nombre de bloc ajoutes
    for(k=0;k<n;k++)
    {
        if((*j)<=98)  //si je suis toujours dans le bloc consernee
        {
            buf->tab[*j]=chaine[k]; // affectation des caract�res de la chaine dans le buffer un a un
            (*j)++;                  // deplacement da,s le buffer
        }
        else                          // si la chaine a inserer depasse le buffer
         {
            ecriredir(fichier,*i,buf);  // ecriture du precedent buffer dans le fichier
            alloc_bloc(fichier);        // alocation d'un nouveau bloc afin de recevoir le reste de la chaine
            buf->tab[0]=chaine[k];      // ecrtiture du kiem caractere de la chaine dans la position 0
            (*j)=1;                     // passage a la position 1
            (*i)=entete(fichier,3);     // deplacement dans les bloc du ficher
            (*cpt)++;                   // incrementation du nombre de bloc alloues
        }
    }
    buf->tab[*j]='\0';                  // fin de la chaine

}







void recherche(Fichier *fichier,int cle,int *trouv,int *i, int *j )
{
    Bloc buf;
    char *ch1=malloc(sizeof(char)*3); // chaine pour recevoir la taille de l'info
    char *ch2=malloc(sizeof(char));   // chaine pour recevoir le champ effac�
    char *ch3=malloc(sizeof(char)*5);   // chaine pour recevoir la cle
    char *chaine=malloc(sizeof(char)*50); //chaine pour recevoir l'info'
    int stop=0;                          // booleen pour l'arret de la recherche
    int bloc,pos;
    (*trouv)=0;                          // initialisdation de trouv a 0
    (*i)=entete(fichier,2);              // positionnement au bloc entete pour une recherche sequentielle
    (*j)=0;                              // premiere position
    if(entete(fichier,1)!=0)             // si le fichier nest pas vide
    {

        liredir(fichier,*i,&buf);     // lecture du premier bloc afin d'entammer le parcours


        while((stop==0) && ((*trouv)==0)) // tant qu'on n`a pas trouve ou qu'on n`est pas a la fin du ficher
        {
            bloc=(*i);   // sauvegarde du bloc a partir de lequel on a commence l'extraction de l'enregistrement
            pos=(*j);    //sauvegarde de la position
            recuperer_chaine(fichier,3,i,j,ch1,&buf);
            recuperer_chaine(fichier,1,i,j,ch2,&buf);
            recuperer_chaine(fichier,5,i,j,ch3,&buf);
            recuperer_chaine(fichier,atoi(ch1),i,j,chaine,&buf); // longueur de l'info est le nombre equivalent a ch1
            int resultat=strcmp(ch2,"f");
            int comp=atoi(ch3);

             if((comp==cle) && (resultat==0)) // si l'info n'est pas effacee logiquement et que la cle correspond
             {

                    (*trouv)=1;   // arret de la recherche
            }
            else
            {
                if(((*i)==entete(fichier,3))&& ((*j)== entete(fichier,4))) // si on est a la fin du fichier on arrete
                {
                     stop=1;
                }
            }
        }
        (*i)=bloc;
        // renvoi du bloc et de la position aux quels on a trouve l'info si elle existe
        (*j)=pos;
    }

}








void insertion(Fichier *fichier, int cle, char *info)
{
    int i,j,trouv=0,cpt;
    Bloc buf;
    char *cle_ch=malloc((sizeof(char))*100);
    recherche(fichier,cle,&trouv,&i,&j);
    if(entete(fichier,1)==0)
      aff_entete(fichier,1,1);
    if(trouv==0)
    {
        cpt=0;
        i=entete(fichier,3);
        j=entete(fichier,4);
      
        liredir(fichier,i,&buf);
        sprintf(cle_ch,"%s","");//convertir en chaine
        concat(cle_ch,cle,info);
        ecrire_chaine(fichier,strlen(cle_ch),&i,&j,cle_ch,&cpt,&buf);
        ecriredir(fichier,i,&buf);
        aff_entete(fichier,3,i);
        aff_entete(fichier,4,j);
         // Mise à jour des entêtes pour refléter la nouvelle position dans la queue et la nouvelle position libre dans le bloc.
    }

    else
    {
        printf("i cle deja existante\n");
    }
}




void suppression_logique(Fichier *fichier, int cle)
{
    int i,j,trouv;
    Bloc buf;
    char *chaine=malloc(sizeof(char)*3);
    recherche(fichier,cle,&trouv,&i,&j);      // recherche de la cle dans le fichier
    if(trouv==1)                                // si la cle a etait trouvee
    {
        liredir(fichier,i,&buf);                       // lecture du bloc dans lequel on a trouve l'info
        recuperer_chaine(fichier,3,&i,&j,chaine,&buf); // recuperation de la chaine correpondant a la taille de l'info
        buf.tab[j]='v';                                // mise du champs efface a vrai
        ecriredir(fichier,i,&buf);                     // reecriture du bloc
        aff_entete(fichier,5,entete(fichier,5)+atoi(chaine)+9); // mise a jour du nombre de caractere supprimes
        printf("\nsuppression logique reussie\n");
    }
    else
    {
        printf("\n   suppression impossible cl� inexistante\n");
    }
}








void afficher_bloc(Fichier *fichier,int i)
{
    Bloc buf;
    liredir(fichier,i,&buf); //lecture du ieme bloc
    printf("le tableau de caracteres de bloc %d contient\n\n",i);
    printf("%s\n",buf.tab); //affichage de la chaine de carcteres contenue
    printf(" le suivant du bloc %d  est %d\n",i,buf.Suiv); // affichage du suivant

}










void afficher_fichier(Fichier *fichier)
{
    int i=entete(fichier,2); // positionnement a la tete du fichier
    int j=0;                  // positionnement au premier caractere
    int stop=0;
    Bloc buf;
    char *ch1=malloc(sizeof(char)*3);
    char *ch2=malloc(sizeof(char));
    char *ch3=malloc(sizeof(char)*5);
    char *ch4=malloc(sizeof(char)*100);
    liredir(fichier,i,&buf);    // lecture du premier bloc
  if(entete(fichier,1)!=0)
  {
    while(!stop)
    {
        recuperer_chaine(fichier,3,&i,&j,ch1,&buf); //on commence a recuperer les enregistrememnt un a un
        recuperer_chaine(fichier,1,&i,&j,ch2,&buf);
        recuperer_chaine(fichier,5,&i,&j,ch3,&buf);
        recuperer_chaine(fichier,atoi(ch1),&i,&j,ch4,&buf);
        if(strcmp(ch2,"f")==0) //. si l'info n'a pas ete supprime logiquement
        {
            printf("%s%s%s%s",ch1,ch2,ch3,ch4); //impression de la totalite de l'enregistrement

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






void suppression_physique(Fichier *fichier,char *nom_physique)
{
     Bloc buf1,buf2;
     Fichier *fichier2;
     int bloc1,bloc2,pos1,pos2,cpt=0;
     char *ch1=malloc(sizeof(char)*3);
     char *ch2=malloc(sizeof(char));
     char *ch3=malloc(sizeof(char)*5);
     char *ch4=malloc(sizeof(char)*50);
     char *chaine=malloc(sizeof(char)*100);
     fichier2=ouvrir(nom_physique,'n');
     bloc1=entete(fichier,2);
     pos1=0;
     aff_entete(fichier2,1,1);
     bloc2=1;
     pos2=0;
     liredir(fichier,bloc1,&buf1);// lecture du premier bloc dans le fichier 1
     int stop=0;
     while(!stop)
     {
        sprintf(chaine,"%s",""); // recuperation des chaines
        recuperer_chaine(fichier,3,&bloc1,&pos1,ch1,&buf1);
        recuperer_chaine(fichier,1,&bloc1,&pos1,ch2,&buf1);
        recuperer_chaine(fichier,5,&bloc1,&pos1,ch3,&buf1);
        recuperer_chaine(fichier,atoi(ch1),&bloc1,&pos1,ch4,&buf1);
        strcat(chaine,ch1);// concatination des chaines obtenues
        strcat(chaine,ch2);
        strcat(chaine,ch3);
        strcat(chaine,ch4);
        if(strcmp(ch2,"f")==0) // si la cle n'a pas etais supprime logiquement on l'ensere dans le noveau ficher
        {
             ecrire_chaine(fichier2,strlen(chaine),&bloc2,&pos2,chaine,&cpt,&buf2);
             aff_entete(fichier2,3,bloc2);
             aff_entete(fichier2,4,pos2);
        }
        if((bloc1==entete(fichier,3))&&(pos1==entete(fichier,4))) // si on est a la fin du ficher source on  arrete
        {
            stop=1;
        }


     }
    ecriredir(fichier2,bloc2,&buf2);

}







int nb_pos (int a)
{
  int cpt = 1;
  while ((a / 10) != 0)
    {
      a = a / 10;
      cpt++;
    }
  return (cpt);
}






int cle_correct (int cle)
{
  int correct = 0;
  if (nb_pos (cle) > 5)

    {
      puts ("cle n'est pas correct");
    }
  return (correct);

}





void creation_fichier(Fichier *fichier,int n)
{
    int k=0,cle;
    char *info=malloc(sizeof(char)*50);
    aff_entete(fichier,1,1);
    for(k=0;k<n;k++)
    {
        printf("\tveuillez introduire la cle que vous voulez inserer\n");
        scanf("%d",&cle);
        printf("\t veuillez introduire l'info que vous voulez inserer\n\n");
        scanf("%s",info);
        if(cle_correct(cle))
           {

            insertion(fichier,cle,info);
           }
    }
}






void afficher_entete(Fichier *fichier)
{
    printf("\nles caracteristiques du ficher sont les suivantes\n--------------------------------------------");
    printf("le nombre total de bloc allouer est   %d\n",entete(fichier,1));
    printf("l'entete du ficher se trouve au bloc    %d\n",entete(fichier,2));
    printf("la queue du ficher se trouve au bloc    %d\n",entete(fichier,3));
    printf("la postion libre dans le bloc de queue est    %d\n",entete(fichier,4));
    printf("le nombre total de caractere supprimer est    %d\n",entete(fichier,5));
}




int main()
{

  char * info=malloc(sizeof(char)*50);
  char *nom_physique=malloc(sizeof(char)*50);
  char mode;
  int trouv,j,i,cle;
  Fichier *fichier;


                            printf("\n\n\n \t\t\t\t\t<<<<<<<<<<CREATION DU FICHIER>>>>>>>>>>\n\n\n");
                            printf("\t veuillez introduire le nom du fichier que vous voulez creer\n\n");
                            scanf("%s",nom_physique);
                            printf("\t veuillez introduire le mode d'ouverture du fichier\n\n");
                            getchar();
                            scanf("%c",&mode);
                            fichier=ouvrir(nom_physique,mode);
                            if(fichier!=NULL)
                            {
                               printf("\n\t\t ouverture reussie\n");
                            }
                            else
                            {
                                printf("echec lors de l'ouverture\n\n");
                            }
                            printf("veuillez introduire le nombre d'enregistrement que vous voulez inserer\n\n");
                            int n;
                            scanf("%d",&n);
                            creation_fichier(fichier,n);



                            printf("\n\n\n \t\t\t\t\t<<<<<<<<<<RECHERCHE DANS LE FICHIER>>>>>>>>>>\n\n\n");
                            printf("veuillez introduire le  nom du fichier dans le quel vous voulez chercher");
                            scanf("%s",nom_physique);
                            fichier=ouvrir(nom_physique,'a');
                            printf("veuillez introduire le la cle que vous voulez rechercher\n\n");
                            scanf("%d",&cle);
                            recherche(fichier,cle,&trouv,&i,&j);
                            if(trouv==1)
                            {
                                printf("cle trouvee au bloc %d  a la position %d\n", i,j);
                            }
                            else
                            {
                                printf("\ncle inexistante\n ");
                            }




                            printf("\n\n\n \t\t\t\t\t<<<<<<<<<<INSERTION DANS LE FICHIER>>>>>>>>>>\n\n\n");
                            printf("veuillez introduire le  nom du fichier dans le quel vous voulez inserer");
                            scanf("%s",nom_physique);
                            fichier=ouvrir(nom_physique,'a');
                            printf("veuillez introduire  la cle que vous voulez inserer\n\n");
                            scanf("%d",&cle);
                            printf("veuillez introduire l'info  que vous voulez inserer\n\n");
                            scanf("%s",info);
                            insertion(fichier,cle,info);





                             printf("\n\n\n \t\t\t\t\t<<<<<<<<<<AFFICHAGE DU FICHIER FICHIER>>>>>>>>>>\n\n\n");
                             printf("veuillez introduire le  nom du fichier dans le quel vous voulez afficher\n\n");
                             scanf("%s",nom_physique);
                             fichier=ouvrir(nom_physique,'a');
                             afficher_fichier(fichier);





                           /*printf("\n\n\n \t\t\t\t\t<<<<<<<<<<SUPPRESSION LOGIQUE DANS LE FICHIER>>>>>>>>>>\n\n\n");
                            printf("veuillez introduire le  nom du fichier dans le quel vous voulez supprimer logiquement");
                            scanf("%s",nom_physique);
                            fichier=ouvrir(nom_physique,'a');
                            printf("veuillez introduire  la cle que vous voulez inserer\n\n");
                            scanf("%d",&cle);
                            suppression_logique(fichier,cle);




                            printf("\n\n\n \t\t\t\t\t<<<<<<<<<<SUPPRESSION PHYSIQUE DANS LE FICHIER>>>>>>>>>>\n\n\n");
                             printf("veuillez introduire le  nom du fichier dans le quel vous voulez supprimer physiquement\n\n");
                             scanf("%s",nom_physique);
                             printf("veuillez introduire le  nom du fichier dans le quel vous voulez recuperer apres suppression");
                             scanf("%s",nom_physique);
                             fichier=ouvrir(nom_physique,'a');

                             printf("la suppression n'est lancée que si la moitie au moin des caracteres sont supprimes logiquement\n\n");
                             if(entete(fichier,5)>=(entete(fichier,1)-1)*50+entete(fichier,4))
                             {
                                 suppression_physique(fichier,nom_physique);
                             }


                             printf("\n\n\n \t\t\t\t\t<<<<<<<<<<AFFICHAGE DU FICHIER FICHIER>>>>>>>>>>\n\n\n");
                             printf("veuillez introduire le  nom du fichier dans le quel vous voulez afficher\n\n");
                             scanf("%s",nom_physique);
                             fichier=ouvrir(nom_physique,'a');
                             afficher_fichier(fichier);



                              printf("\n\n\n \t\t\t\t\t<<<<<<<<<<AFFICHAGE DES CARACTERISTIQUES  DU  FICHIER>>>>>>>>>>\n\n\n");
                             printf("veuillez introduire le  nom du fichier dans le quel vous voulez reordonner \n\n");
                             scanf("%s",nom_physique);
                             fichier=ouvrir(nom_physique,'a');
                             afficher_entete(fichier);
                             fermer(fichier);
                             */
                            fermer(fichier);


                      return 0;
                      }
