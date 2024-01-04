
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
        printf("insertion impossible cle deja existante\n");
    }
}



// avecun buffer de 3


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_BUF 512 // Taille du buffer, ajustez selon vos besoins

typedef struct {
    int suiv;
    char chaine_longueur[3];
    char type;
    char enreg[20];
} Buf;



void Inserer(char *NomFichier, char *Cle, char *Enreg) {
    int Trouv, i, j;
    Recherche(NomFichier, Cle, &Trouv, &i, &j);

    if (Trouv) {
        printf("Insertion impossible\n");
    } 
    else {
        FILE *F;
        Buf buf;

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
