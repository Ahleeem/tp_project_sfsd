
void insertion(Fichier *fichier, int cle, char *info)
{
    int i,j,trouv=0,cpt;
    Buffer buf;
    
    char *cle_ch=malloc((sizeof(char))*100);            // la chaine qui va contenir l'enregistrement qui va etre inseré
   
    recherche(fichier,cle,&trouv,&i,&j);               // recherche de la cle pour eviter les  doublon
    if(entete(fichier,1)==0)                           // si la cle n'a  pas été trouvé alors on insere dans le premier bloc
      aff_entete(fichier,1,1);
    if(trouv==0)
    {
        cpt=0;
        i=entete(fichier,3);                                             // positionnement dans le queue
        j=entete(fichier,4);                                             // positionnnement a la position libre dans le bloc de queue
        liredir(fichier,i,&buf);                                          // lecture du  bloc
        sprintf(cle_ch,"%s","");                                         //convertir en chaine
        concat(cle_ch,cle,info);                                       // creation de l'enregistrement
        ecrire_chaine(fichier,strlen(cle_ch),&i,&j,cle_ch,&cpt,&buf);   // ecriture de la chaine construite
        
        //ecriredir(fichier,i,&buf);                                    // ecriture du dernier bloc 
        aff_entete(fichier,3,i);                                        // mise a jour de la quee
        aff_entete(fichier,4,j);                                        // mise a jour de la position libre
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
void ecrire_chaine(Fichier *fichier,int n , int *i, int *j, char chaine[],int *cpt,Buffer *buf)
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
            ecriredir(fichier,*i,buf);  // ecriture du precedent buffer dans le fichier
            alloc_bloc(fichier);        // allocation d'un nouveau bloc afin de recevoir le reste de la chaine
            buf->tab[0]=chaine[k];      // ecrtiture du k°eme caractère de la chaine dans la position 0
            (*j)=1;                     // passage a la position 1
            (*i)=entete(fichier,3);     // deplacement dans les bloc du ficher
           (*cpt)++;                   // incrementation du nombre de bloc alloues
        }
    }
    buf->tab[*j]='\0';                  // fin de la chaine

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
    strcat(chaine,ch_f);                          // construction de la chaine finale avec l'ordre suivant taille efface cle info
}

