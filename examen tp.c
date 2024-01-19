
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Bloc Buffer;  // declaration du tampon
typedef struct Fichier
{
    FILE *fich;
    int entete[4];
}Fichier;


void suppression_physique_L7OV7C(Fichier *fichier, char *nom_physique)
{
    Buffer buf1, buf2;
    Fichier *fichier2;
    int bloc1, bloc2, pos1, pos2, cpt = 0;
    char *ch1 = malloc(sizeof(char) * 3);
    char *ch2 = malloc(sizeof(char));
    char *ch3 = malloc(sizeof(char) * 5);
    char *ch4 = malloc(sizeof(char) * 50);
    char *chaine = malloc(sizeof(char) * 100);
    fichier2 = ouvrir(nom_physique, 'n'); // ouverture du fichier
    bloc1 = entete(fichier, 2);
    pos1 = 0;
    aff_entete(fichier2, 1, 1);
    bloc2 = 1;
    pos2 = 0;
    liredir(fichier, bloc1, &buf1); // lecture du premier bloc dans le fichier 1
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

        liredir(fichier, bloc1, &buf1); // lecture du bloc suivant
    }

    ecriredir(fichier2, bloc2, &buf2);
    fermer(fichier2);
}
void fermer(Fichier *fichier)
{
    // Repositionnement du curseur en debut de fichier
    rewind(fichier->fich);

    // Utilisation d'un tampon temporaire pour eviter le chevauchement
    Buffer buf_temp;
    liredir(fichier, 0, &buf_temp);

    // ecriture de l'entete dans le tampon temporaire
    memcpy(buf_temp.tab, &(fichier->entete), sizeof(Entete));

    // Reecriture du tampon temporaire dans le fichier
    ecriredir(fichier, 0, &buf_temp);

    // Fermeture du fichier
    fclose(fichier->fich);
}

void liredir(Fichier *fichier, int i, Buffer *buf)
{
    // Utilisation d'un tampon temporaire pour eviter le chevauchement
    Buffer buf_temp;

    // Positionnement au debut du bloc numero i
    fseek(fichier->fich, sizeof(Entete) + sizeof(Tbloc) * (i - 1), SEEK_SET);

    // Lecture d'un bloc de caracteres correspondant a la taille du bloc dans le tampon temporaire
    if (fread(&buf_temp, sizeof(Buffer), 1, fichier->fich) != 1) {
        // Gestion d'erreur lors de la lecture
        perror("Erreur lors de la lecture du bloc");
        // Autres actions necessaires en cas d'erreur
        return;
    }

    // Copie du contenu du tampon temporaire dans le tampon de sortie
    memcpy(buf, &buf_temp, sizeof(Buffer));

    // Repositionnement au debut du fichier
    rewind(fichier->fich);
}
