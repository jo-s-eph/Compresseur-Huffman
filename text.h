#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tree.h"
#include "huffman.h"
// #include "list.h"

// Permet d'obtenir le contenu d'un fichier à partir de son nom.
char* lireFichier(const char *nom);

// Retourne la taille en octet d'un fichier à parir de son nom.
long getFileSize(const char *nom);

// Permet d'obtenir le nom d'un fichier sans son extension (ex : "file.txt").
char* getFileName(const char *nom);

// Retourne un tableau d'occur. de chaque case i qui contient le nb. d'occur. de (char)i.
int* tab_occurences(char *texte);

// Retourne le nombre de cases non-vides dans un tableau de 128 entiers.
int nb_char_unique(int ascii_o[128]);

//Convertit une entrée Huffman en une chaîne de caractères binaires.
char* binary2Char(EntreeHuffman *entree);

// Enregistre l'arbre binaire dans un fichier.
void enregistrerArbreBinaire(FILE *fichier, noeud_t *noeud);

// Exporte l'arbre dans un fichier.
void exportArbre(const char *nomFichier, noeud_t *racine);

// Importe l'arbre binaire à partir d'un fichier.
noeud_t* importerArbreBinaire(FILE *fichier);

// Importe le texte à partir d'un fichier compressé et le stocke dans un fichier de sortie.
void importTexte(const char *nomFichierCompresse, const char *nomFichierSortie);

#endif // TEXT_H