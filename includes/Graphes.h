//
// Created by mathe on 08/10/2023.
//

#ifndef ISOMETRICENGINE_GRAPHES_H
#define ISOMETRICENGINE_GRAPHES_H

#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "allegro.h"

// dijkstra
void myDijkstra(graphe_t *graphe, int sDep, int numCompo);
int *myDSF(graphe_t *g, int sdep, int *pred, int numCompo);

// Graphe
graphe_t *loadGraphe();
void freeGraphe(graphe_t *graphe);

// Display
void displayGrapheInfo(graphe_t *graphe);
int *getWay(graphe_t *graphe, int sDep, int sFin,int *nbCheminTrouve);

void handleMalloc(void *var);

#endif //ISOMETRICENGINE_GRAPHES_H
