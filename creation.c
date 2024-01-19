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

//permet de mettre a jour les elements de l'entete du ficher
void aff_entete(Fichier *fichier,int i , int valeur)
{

    switch(i)
    {
        case 1:
        {
            fichier->entete.nbbloc=valeur;        // nombre total de bloc alloués dans le fichier
        }break;
         case 2:
        {
            fichier->entete.tete=valeur;          // numero du bloc representatnt la tete du fichier
        }break;
         case 3:
        {
            fichier->entete.queue=valeur;        // numero du bloc representatnt la tete du fichier
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

