#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "text.h"
#include "tas.h"


typedef struct {
    char caractere;    // Le caractère ASCII
    unsigned long code; // Le code Huffman correspondant
    int longueurCode;   // La longueur du code Huffman
} EntreeHuffman;

typedef struct {
    EntreeHuffman t[128]; // Tableau de pointeurs vers EntreeHuffman
    int taille;          // Taille de la table
} table_hm;

noeud_t* huffman(tas_t *tas);
// Permet de créer une forêt à partir d'un tableau
tas_t* creerforets(int ascii_o[128]);

// Parcours un arbre de Huffman jusqu'à la racine correspondant au caractère recherché
void encodeChar(noeud_t* huff, int arr[], int top, char target, int* found, char* res);

// Remplit une table de codage à partir de son arbre de Huffman
void creerTable(noeud_t* huff, unsigned long code, int longueur, table_hm* tableHM);

// Encode un texte à partir de son arbre de Huffman et de sa table de codage
char* encoderTexte(noeud_t* abr, table_hm* tbl, char* text);

// Convertit une chaîne de caractères en binaire
unsigned char* ChainetoBinaire(const char* chaine, size_t* taille);

// Exporte le texte encodé dans un fichier
void exportTexte(noeud_t* abr, table_hm* tbl, char* texte, const char* nomFichier);

#endif // HUFFMAN_H