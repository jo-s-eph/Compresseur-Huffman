#ifndef TREE_H
#define TREE_H

typedef struct s_noeud_t {
    int v; // Le nombre d'occurence 
    char c; // Le caractère ASCII
    struct s_noeud_t* g; // Fils gauche
    struct s_noeud_t* d; // Fils droit
} noeud_t;

// Creer un arbre avec fils gauche "gau" et fils droit "droit" avec les valeur v et c
noeud_t* consA(noeud_t* gau, noeud_t* droit, int v, char c);

// libere la memoire de l'arbre donnée
void libererA(noeud_t *a);

// donne la hauteur d'un arbre
int hauteur(noeud_t* a);

// donne le nombre de feuille de l'arbre
int nbFeuilles(noeud_t* a);

// Compare deux arbre par leur valeur v
int comparerA(const void *a, const void *b);

// Retourne une feuille comportant une lettre ainsi que sa sa fréquence.
noeud_t* node_occurence(char lettre, int ascii_o[128]);

#endif // TREE_H
