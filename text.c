#include "text.h"

/*
Permet d'obtenir le contenu d'un fichier à partir de son nom.
Le contenu est renvoyé dans une chaîne de caractères. 
*/
char* lireFichier(const char *nom){
    FILE *f = fopen(nom, "r");
    char *res = NULL;
    long taille;

    if (!f) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    // On se place à la fin du texte pour recup la taille du texte
    fseek(f, 0, SEEK_END);
    taille = ftell(f);
    // On se replace au début pour la lecture
    rewind(f);

    // Allouer de la mémoire pour contenir tout le texte
    res = (char *)malloc((taille + 1) * sizeof(char));
    // Lire le fichier dans la mémoire
    fread(res, sizeof(char), taille, f); 

    // Ajouter le caractère de fin de chaîne
    res[taille] = '\0';

    fclose(f);
    return res;
}

/*
Retourne la taille en octet d'un fichier à parir de son nom.
La taille est renvoyée dans un entier signé. 
*/
long getFileSize(const char *nom){
    long taille = 0;
    FILE *file = fopen(nom, "rb"); // Ouvrir le fichier en mode lecture binaire

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1; // Retourner -1 pour indiquer une erreur
    }

    fseek(file, 0, SEEK_END); // Se positionner à la fin du fichier
    taille = ftell(file);   // Obtenir la taille du fichier
    fclose(file);             // Fermer le fichier

    return taille; // Retourner la
}

/*
Permet d'obtenir le nom d'un fichier sans son extension (ex : "file.txt").
Le nom est renvoyé dans une chaîne de caractères. 
*/
char* getFileName(const char *nom){
    char* res = malloc(256);
    char *posDuPoint = strrchr(nom, '.');

    if (posDuPoint != NULL){
        int t = posDuPoint - nom;
        strncpy(res, nom, t);
        res[t] = '\0';
    }
    return res;
}


/*
Retourne un tableau d'occurence de chaque case i qui contient le nombre d'occurence 
de (char)i. 
*/ 

int* tab_occurences(char *texte){
    int* ascii_tab = malloc(128 * sizeof(int));
    for (int i = 0; i < 128; i++) ascii_tab[i] = 0;

    for (int i = 0; texte[i] != '\0'; i++)
        ascii_tab[(int)texte[i]]++;
    return ascii_tab;
}

/*
Retourne le nombre de characteres uniques dans un tableau ASCII,
soit le nombre de cases non-vides.
*/

int nb_char_unique(int ascii_o[128]){
    int res = 0;
    for (int i = 0; i < 128; i++)
        if (!ascii_o[i]) res++;
    return res;
}

/*
Convertit une entrée Huffman en une chaîne de caractères binaires.
*/
char* binary2Char(EntreeHuffman *entree) {
    if (entree == NULL) NULL;
    // Calculer la taille du code Huffman
    int size = entree->longueurCode;
    // Allouer de la mémoire pour la chaîne de caractères
    char* res = malloc(size + 1);
    // Ajouter le caractère de fin de chaîne
    res[size] = '\0'; 

    // Convertir le code Huffman en une chaîne de caractères binaires
    for (int i = 0; i < size; i++)
        res[i] = ((entree->code >> (size - 1 - i)) & 1) ? '1' : '0';

    return res;
}


void enregistrerArbreBinaire(FILE *fichier, noeud_t *noeud) {
    if (noeud == NULL) {
        char indicateur = 0; // indicateur 0 pour les noeuds nuls
        fwrite(&indicateur, sizeof(char), 1, fichier);
        return;
    }

    if (noeud->g == NULL && noeud->d == NULL) {
        char indicateur = 1; // indicateur 1 pour les feuilles
        fwrite(&indicateur, sizeof(char), 1, fichier);
        fwrite(&(noeud->c), sizeof(char), 1, fichier);
        fwrite(&(noeud->v), sizeof(int), 1, fichier);
    } else {
        char indicateur = 2; // indicateur 2 pour les noeuds
        fwrite(&indicateur, sizeof(char), 1, fichier);
        enregistrerArbreBinaire(fichier, noeud->g);
        enregistrerArbreBinaire(fichier, noeud->d);
    }
}

/*
Exporte l'arbre dans un fichier.
*/
void exportArbre(const char *nomFichier, noeud_t *racine) {
    FILE *fichier = fopen(nomFichier, "wb");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Calculer la taille de l'arbre (en octets) et l'écrire en en-tête
    long positionDebut = ftell(fichier); // Mémoriser la position actuelle
    fseek(fichier, sizeof(long), SEEK_CUR); // Réserver de la place pour la taille

    enregistrerArbreBinaire(fichier, racine); // Écrire l'arbre

    long positionFin = ftell(fichier); // Mémoriser la nouvelle position
    long tailleArbre = positionFin - positionDebut - sizeof(long);

    fseek(fichier, positionDebut, SEEK_SET); // Revenir à la position de début
    fwrite(&tailleArbre, sizeof(long), 1, fichier); // Écrire la taille

    fseek(fichier, positionFin, SEEK_SET); // Retourner à la fin de l'arbre

    fclose(fichier);
}



/*
Importe l'arbre binaire à partir d'un fichier.
*/
noeud_t* importerArbreBinaire(FILE *fichier) {
    char indicateur;
    if (fread(&indicateur, sizeof(char), 1, fichier) != 1) {
        return NULL;
    }
    if (indicateur == 0) return NULL; // indicateur 0 donc noeud nul

    else if (indicateur == 1) { // indicateur 1 pour donc feuille
        char c;
        int v = 0;
        if (fread(&c, sizeof(char), 1, fichier) != 1 || fread(&v, sizeof(int), 1, fichier) != 1) perror("Erreur de lecture du fichier");
        return consA(NULL, NULL, v, c);

    } else if (indicateur == 2) { // indicateur 2 pour donc noeud

        noeud_t* g = importerArbreBinaire(fichier); // Appels recursifs pour les fils
        noeud_t* d = importerArbreBinaire(fichier);// "
        return consA(g, d, 0, '\0');
    } else {
        perror("Indicateur inconnu");
        exit(EXIT_FAILURE);
    }
}

/*
Importe le texte à partir d'un fichier compressé et le stocke dans un fichier de sortie.
*/
void importTexte(const char *nomFichierCompresse, const char *nomFichierSortie){
    FILE *fichierCompresse = fopen(nomFichierCompresse, "rb");
    if (!fichierCompresse) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    long tailleArbre = 0;
    fread(&tailleArbre, sizeof(long), 1, fichierCompresse);

    // On recupere la taille en octet de l'arbre
    // printf("Taille octets de l'arbre : %ld\n",tailleArbre);

    // On  importe ce dernier donc le curseur est au debut du texte codé
    noeud_t* racine = importerArbreBinaire(fichierCompresse);
    noeud_t* courant = racine;

    FILE *fichierSortie = fopen(nomFichierSortie, "w"); // Ouvrir le fichier de sortie en mode écriture
    if (fichierSortie == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        fclose(fichierCompresse);
        return;
    }
    int bit;
    char c;

    while (!feof(fichierCompresse)) { // i.e tant qu'on est pas à la fin
        c = fgetc(fichierCompresse); // on recup le premier caractère

        for (int i = 0; i < 8; i++) {
            bit = c & (1 << (7 - i)); // Extraire le bit i-ème

            if (bit == 0)
                courant = courant->g; // Gauche pour 0
            else 
                courant = courant->d; // Droite pour 1
        
            if (courant->g == NULL && courant->d == NULL) { // Vérifier si c'est une feuille
                // printf("%c",courant->c); debug
                fputc(courant->c, fichierSortie); // Écrire le caractère dans le fichier de sortie
                courant = racine; // Retourner à la racine de l'arbre
            }
        }
    }

    free(racine);
    fclose(fichierCompresse); // Fermer le fichier compressé
    fclose(fichierSortie); // Fermer le fichier de sortie
}