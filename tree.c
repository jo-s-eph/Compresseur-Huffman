#include "tree.h"

/*
Herité du TP-5 sur les arbres.
*/

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y)) // nécessaire pour la fonction hauteur par ex

noeud_t* consA(noeud_t* gau, noeud_t* droit, int v, char c){
    noeud_t *res = malloc(sizeof(noeud_t));
    res->v = v;
    res->c = c;
    res->d = droit;
    res->g = gau;
    return res;
}

void libererA(noeud_t *a){
    if (a != NULL){
        if (a->g != NULL) libererA(a->g);
        if (a->d != NULL) libererA(a->d);
        free(a);
    }
}

int hauteur(noeud_t* a){
    if (a == NULL) return -1;
    else return 1 + MAX(hauteur(a->g), hauteur(a->d));
}

int nbFeuilles(noeud_t* a){
    if (a->g == NULL && a->d == NULL) return 1;
    else return nbFeuilles(a->g) + nbFeuilles(a->d);
}

int comparerA(const void *a, const void *b) {
    noeud_t *noeudA = *(noeud_t**)a;
    noeud_t *noeudB = *(noeud_t**)b;
    return (noeudA->v - noeudB->v);
}

// Retourne une feuille comportant une lettre ainsi que sa sa fréquence.
noeud_t* node_occurence(char lettre, int ascii_o[128]){
    return consA(NULL,NULL,(int)ascii_o[(int)lettre],lettre);
}