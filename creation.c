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

