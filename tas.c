#include "tas.h"

/*
Herité du TP-6 sur les TAS.
*/

tas_t* creerTAS(int capacity) {
    tas_t* tas = (tas_t *)malloc(sizeof(tas_t));
    tas->tableau = (noeud_t **)malloc(capacity * sizeof(noeud_t));
    tas->capacity = capacity;
    tas->size = 0;
    return tas;
}

void swap_node(noeud_t **a, noeud_t **b) {
    noeud_t *temp = *a;
    *a = *b;
    *b = temp;
}

void equilibrerTAS(tas_t *tas, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < tas->size && tas->tableau[left]->v < tas->tableau[smallest]->v)
        smallest = left;

    if (right < tas->size && tas->tableau[right]->v < tas->tableau[smallest]->v)
        smallest = right;

    if (smallest != idx) {
        swap_node(&tas->tableau[idx], &tas->tableau[smallest]);
        equilibrerTAS(tas, smallest);
    }
}


void insererTAS(tas_t *tas, noeud_t *element) {
    if (tas->size == tas->capacity) {
        printf("Tas plein\n");
        return;
    }

    tas->size++;
    int i = tas->size - 1;
    tas->tableau[i] = element;

    while (i != 0 && tas->tableau[(i - 1) / 2]->v > tas->tableau[i]->v) {
        swap_node(&tas->tableau[i], &tas->tableau[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}


noeud_t* extractMin(tas_t *tas) {
    if (tas->size <= 0) {
        printf("Tas vide\n");
        return NULL;
    }
    if (tas->size == 1) {
        tas->size--;
        return tas->tableau[0];
    }

    noeud_t *root = tas->tableau[0];
    tas->tableau[0] = tas->tableau[tas->size - 1];
    tas->size--;
    equilibrerTAS(tas, 0);
    return root;
}

void libererTAS(tas_t *tas) {
    free(tas->tableau);
    free(tas);
}