#ifndef TAS_H
#define TAS_H
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

typedef struct s_TAS_t {
    noeud_t **tableau;     // Tableau pour stocker les éléments du tas
    int capacity;      // Capacité maximale du tas
    int size;          // Nombre actuel d'éléments dans le tas
} tas_t;


void equilibrerTAS(tas_t *tas, int idx);
void insererTAS(tas_t *tas, noeud_t *element);
void libererTAS(tas_t *tas);
void swap_node(noeud_t **a, noeud_t **b);
noeud_t* extractMin(tas_t *tas);

#endif // TAS_H