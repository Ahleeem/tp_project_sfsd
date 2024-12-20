void  alloc_bloc(Fichier *fichier)
{

        Buffer *buf=malloc(sizeof(Buffer));        // allocation du Buffer
        liredir(fichier,entete(fichier,3),buf);    // lecture du bloc correspondant a la queue
        buf->suivant=entete(fichier,1)+1;          // mise a jour du suivant de la queue au bloc correspondant a la nouvelle queue
        ecriredir(fichier,entete(fichier,3),buf);  // ecriture du bloc de queue dans le fichier
        aff_entete(fichier,3,entete(fichier,1)+1);// mise a jour du numero du bloc correspondant a la nouvelle queue dans l'entete
        buf->suivant=-1;                          // mise a jour du suivant a nill
        sprintf(buf->tab,"%s","");                // vider la chaine du buffer  apres convertion
        ecriredir(fichier,entete(fichier,3),buf); // ecriture du buffer dans le bloc representatnt la nouvelle queue
        aff_entete(fichier,1,entete(fichier,1)+1); // incrémentation du nombre de bloc alloués
}



void libererBloc(Fichier *f, int i) {
    Bloc buffer;
    lireBloc(f, i, &buffer);    // Lire le bloc à libérer
    free(buffer.tab);           // Libérer la mémoire allouée pour le tableau
    buffer.nb = entete(f, 1);    // Le bloc suivant est l'ancien premier bloc libre
    ecrireBloc(f, i, &buffer);   // Écrire le bloc libéré dans le fichier
    affecterEntete(f, 1, i);      // Le nouveau premier bloc libre est le bloc libéré
}


