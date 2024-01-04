
void insertion_L7OV7C(L7OV7C *fichier, int cle, char *info)
{
    int i,j,trouv=0,cpt; // declaration des variables
    Buffer buf;
    char *cle_ch=malloc((sizeof(char))*100);            // la chaine qui va contenir l'enregistrement qui va etre inseré
   
    recherche_L7OV7C(fichier,cle,&trouv,&i,&j);          // recherche de la cle pour eviter les  doublon
    if(entete(fichier,1)==0)                           // si la cle n'a  pas été trouvé alors on insere dans le premier bloc
      aff_entete(fichier,1,1);
    if(trouv==0)
    {
        cpt=0;
        i=entete(fichier,3);                                             // positionnelent dans le queue
        j=entete(fichier,4);                                             // positionnnement a la position libre dans le bloc de queue
        liredir(fichier,i,&buf); // lecture du  bloc
        sprintf(cle_ch,"%s","");
        concat(cle_ch,cle,info); // creation de l'enregistrement
        ecrire_chaine(fichier,strlen(cle_ch),&i,&j,cle_ch,&cpt,&buf);   // ecriture de la chaine construite
        ecriredir(fichier,i,&buf);                                      // ecriture du dernier bloc en cas de chevauchement
        aff_entete(fichier,3,i);                                        // mise a jour de la quee
        aff_entete(fichier,4,j);                                        // mise a jour de la position libre
    }
    else
    {
        printf(" la cle existe deja\n");
    }
}


/***************/
//lire inas
//ecrire may
//ouvrir may
/*****************/




/**********affecter entete*****************************/
//permet de mettre a jour les elements de l'entete du ficher
void aff_entete(L7OV7C *fichier,int i , int valeur)
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
int entete(L7OV7C *fichier, int i)
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







/********************ecrirechaine*********************/
void ecrire_chaine(L7OVC *fichier,int n , int *i, int *j, char chaine[],int *cpt,Buffer *buf)
{

    int k=0;
    (*cpt)=0;     // nombre de bloc ajoutés
    for(k=0;k<n;k++)   // k pourn le deplacement dans la chaine
    {
        if((*j)<=98)  //si je suis toujours dans le bloc conserné
        {
            buf->tab[*j]=chaine[k]; // affectation des caractères de la chaine dans le buffer un a un
            (*j)++;                  // deplacement da,s le buffer
        }
        else                          // si la chaine a inserer depasse le buffer
         {
            ecriredir(fichier,*i,buf);  // ecriture du precedent buffer dans le fichier
            alloc_bloc(fichier);        // alocation d'un nouveau bloc afin de recevoir le reste de la chaine
            buf->tab[0]=chaine[k];      // ecrtiture du kiem caractère de la chaine dans la position 0
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
    strcat(chaine,ch_f);                          // constructu=ion de la chaine finale avec l'ordre suivant taille efface cle info
}





















/*
void Inserer(char *NomFichier, char *Cle, char *Enreg) {
   
    Recherche(NomFichier, Cle, &Trouv, &i, &j);

    if (Trouv) {
        printf("Insertion impossible\n");
    } 
    else {
        FILE *F;
        bloc buf;

        Ouvrir(F, NomFichier, 'A');
        i = Entete(F, 2);
        j = Entete(F, 3);

        sprintf(buf.chaine_longueur, "%03d", (int)strlen(Enreg));//convertir la longeur en chaine de 3 car
        Ecrire_chaine(3, buf.chaine_longueur, i, j);//copier chaine long dans le buf apartir de j
        buf.type = 'N';
        Ecrire_chaine(1, &buf.type, i, j);
        Ecrire_chaine(strlen(Enreg), Enreg, i, j);

        buf.suiv = -1;
        EcrireDir(F, i, buf);

        if (i != Entete(F, 2)) {
            AFF_Entete(F, 2, i);
        }

        AFF_Entete(F, 3, j);

        Fermer(F);
    }
}
*/

/*
void inseretion ( )
{
bool trouv;
int i,j,k;



Ouvrir( F,nomfich, 'A')
// on commence par rechercher la clé e.cle avec le module précédent pour localiser l'emplacement (i,j)
// où doit être insérerer dans le fichier.
i=entete(F,2)
 j=entete(F,3);

Rech( e.cle, nomfich, trouv, i, j )

SI ( Non trouv )         // e doit être inséré dans le bloc i à la position j
continu ← vrai            // en décalant les enreg j, j+1, j+2, ... vers le bas
// si i est plein, le dernier enreg de i doit être inséré dans i+1

TQ ( continu et i ≤ entete(F,1) ) // si le bloc i+1 est aussi plein son dernier enreg sera

LireDir( F, i, buf ) // inséré dans le bloc i+2, etc ... donc une boucle TQ.
// avant de faire les décalages, sauvegarder le dernier enreg dans une var x ...
x ← buf.tab[ buf.NB ]
// décalage à l'intérieur de buf ...
k ← buf.NB

TQ (k > j)
buf.tab[ k ] ← buf.tab[ k-1 ] ; k ← k-1
FTQ

// insérer e à la pos j dans buf ...
buf.tab[ j ] ← e
// si buf n'est pas plein, on remet x à la pos NB+1 et on s'arrête ...

SI ( buf.NB < b ) // b est la capacité max des blocs (une constante)
buf.NB ← buf.NB+1 ; buf.tab[ buf.NB ] ← x
EcrireDir( F, i, buf )
continu ← faux

SINON // si buf est plein, x doit être inséré dans le bloc i+1 à la pos 1 ...
EcrireDir( F, i, buf )
i ← i+1 ; j ← 1
e ← x // cela se fera (l'insertion) à la prochaine itération du TQ
FSI // non ( buf.NB < b )

FTQ

// si on dépasse la fin de fichier, on rajoute un nouveau bloc contenant un seul enregistrement e
SI i > entete( F, 1 )
buf.tab[1] ← e ; buf.NB ← 1
EcrireDir( F, i, buf ) // il suffit d'écrire un nouveau bloc à cet emplacement
Aff-entete( F, 1, i ) // on sauvegarde le num du dernier bloc dans l'entete 1
FSI
}
Aff-entete( F, 2 , entete(F,2)+1 ) // on incrémente le compteur d'insertions
FSI

Fermer( F )
FIN // insertion
*/
