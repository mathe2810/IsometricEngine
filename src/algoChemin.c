//
// Created by mathe on 08/10/2023.
//
#include "../includes/Graphes.h"

int *myDSF(graphe_t *g, int sdep, int *pred, int numCompo)
{
    arc_t *tmpArc;
    int incrementation = 0;

    g->sommets[sdep]->compoConnex = numCompo;

    tmpArc = g->sommets[sdep]->arc;
    while (tmpArc != NULL) {
        if (pred[tmpArc->sommet->id] == -1) {
            pred[tmpArc->sommet->id] = sdep;
            pred = myDSF(g, tmpArc->sommet->id, pred, numCompo);
        }
        tmpArc = tmpArc->next;
    }

    return pred;
}

int allVisited(sommet_t **sommets, int ordre, int numCompo)
{
    for (int i = 0; i < ordre; i++)
        if (!sommets[i]->couleur && sommets[i]->compoConnex == numCompo) // Verifie si tous les sommets visités
            return 0;
    return 1;
}

int sortDist(graphe_t *graphe, int *currNode, int numCompo)
{
    for (int i = 0, dist = MAX_INT; i < graphe->ordre; i++) {
        if (graphe->sommets[i]->compoConnex == numCompo && !graphe->sommets[i]->couleur && graphe->sommets[i]->dist < dist) {
            dist = graphe->sommets[i]->dist;
            *currNode = i;
        }
    }

    return graphe->sommets[*currNode]->dist;
}

void checkNeighbor(graphe_t *graphe, int currNode, int currDist)
{
    arc_t *tmpArc = graphe->sommets[currNode]->arc;

    while (tmpArc != NULL) {
        if (!tmpArc->sommet->couleur && currDist + tmpArc->w < tmpArc->sommet->dist) {
            tmpArc->sommet->dist = currDist + tmpArc->w;
            tmpArc->sommet->pred = currNode;
        }
        tmpArc = tmpArc->next;
    }
}

void myDijkstra(graphe_t *graphe, int sDep, int numCompo)
{
    int currNode = sDep;
    int lastNode;
    int currDist = 0;

    graphe->sommets[sDep]->dist = 0;

    while (!allVisited(graphe->sommets, graphe->ordre, numCompo)) {
        graphe->sommets[currNode]->couleur = 1; // Marquage du sommet courant

        checkNeighbor(graphe, currNode, currDist); // Parcours de chaque sommets liés au sommet courant

        lastNode = currNode;
        currDist = sortDist(graphe, &currNode, numCompo); // Algo de trie pour choisir le sommet avec le chemin le plus court
    }
}