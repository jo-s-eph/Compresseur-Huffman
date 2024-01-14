#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "text.c"
#include "tree.c"
#include "tas.c"
#include "huffman.c"

void user_compresser(){

    clock_t debut, fin;
    double temps_ecoule;
    unsigned long te1 = 0;
    int te2 = 0;

    printf("----------------------------------------\n");
    char nomFichier[256];
    printf("Entrez le nom du fichier à compresser : ");
    scanf("%255s", nomFichier);
    debut = clock();
    char *contenuFichier = lireFichier(nomFichier);
    long TailleAvant = getFileSize(nomFichier);
    printf("Taille de '%s' : %ld octets\n", nomFichier, getFileSize(nomFichier));

    printf("- Création de la table d'occurences...\n");
    int* ascii_o = malloc(128 * sizeof(int));
    tab_occurences(contenuFichier,ascii_o);
    // ascii_o = tab_occurences(contenuFichier);

    printf("- Création de l'arbre de HuffMan...\n");
    tas_t* forets = creerforets(ascii_o);
    noeud_t* a_de_huffman = huffman(forets);

    printf("- Création de la table de codage...\n");
    table_hm* tableHuffman = malloc(sizeof(table_hm));
    creerTable(a_de_huffman,te1,te2,tableHuffman);

    printf("- Encodage du texte...\n\n");

    char* nvNomFichier = getFileName(nomFichier);
    strcat(nvNomFichier,".b");
    
    exportArbre(nvNomFichier,a_de_huffman);
    exportTexte(a_de_huffman,tableHuffman,contenuFichier,nvNomFichier);

    fin = clock();
    temps_ecoule = ((double) (fin - debut)) / CLOCKS_PER_SEC;
    long TailleApres = getFileSize(nvNomFichier);
    printf("Succès ! Résultat enregistré dans le fichier '%s'\n",nvNomFichier);
    printf("Taille de '%s' : %ld octets\n", nvNomFichier, getFileSize(nvNomFichier));
    printf("Temps de compression : %.0f ms\n", temps_ecoule*1000);
    printf("Taux de compression : %.2f %%\n", (100.0 - ((TailleApres * 100)/(float)TailleAvant)));
    printf("----------------------------------------\n");

    free(contenuFichier);
    free(ascii_o);
    free(forets);
    free(a_de_huffman);
    free(tableHuffman);

}

void user_decompresser(){

    clock_t debut, fin;
    double temps_ecoule;

    printf("----------------------------------------\n");
    char nomFichier[256];
    printf("Entrez le nom du binaire à décompresser : ");
    scanf("%255s", nomFichier);
    printf("Taille de '%s' : %ld octets\n", getFileName(nomFichier), getFileSize(nomFichier));

    debut = clock();
    printf("- Décompression...\n\n");

    char nvNomFichier[256] = "r_";
    strcat(nvNomFichier,getFileName(nomFichier));
    strcat(nvNomFichier,".txt");

    importTexte(nomFichier,nvNomFichier);

    fin = clock();
    temps_ecoule = ((double) (fin - debut)) / CLOCKS_PER_SEC;
    printf("Succès ! Résultat enregistré dans le fichier '%s'\n",nvNomFichier);
    printf("Taille de '%s' : %ld octets\n", nvNomFichier, getFileSize(nvNomFichier));
    printf("Temps de décompression : %.0f ms\n", temps_ecoule*1000);
    printf("----------------------------------------\n");
}

void nettoyerBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int main() {
    printf("\n\n========================================\n");
    printf("======== COMPRESSEUR DE HUFFMAN ========\n");
    printf("========================================\n");
    
    char choixUser[100];
    int continuer = 1;

    while (continuer) {
        printf("'C' pour compresser un fichier\n'D' pour décompresser un binaire\n'Q' pour quitter\nQue souhaitez-vous faire ? : ");
        fgets(choixUser, sizeof(choixUser), stdin);
        
        if (choixUser[0] == 'C' || choixUser[0] == 'c')
            user_compresser();
        else if (choixUser[0] == 'D' || choixUser[0] == 'd')
            user_decompresser();
        else if (choixUser[0] == 'Q' || choixUser[0] == 'q')
            continuer = 0;
        else
            printf("Choix inconnu, réessayez.\n");

        nettoyerBuffer();
    }

    return 0;
}