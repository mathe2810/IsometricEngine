//
// Created by mathe on 08/10/2023.
//
#include "../includes/Graphes.h"

void getWay(graphe_t *graphe, int sDep, int sFin)
{
    int currNode = sFin;

    if (graphe->sommets[sFin]->dist == MAX_INT) {
        printf("\nLe sommet final est dans une autre composante connexe\n");
        return;
    }

    printf("\nDistance: %d | Chemin:", graphe->sommets[sFin]->dist);

    while (currNode != sDep) {
        printf(" %d <-", currNode);
        currNode = graphe->sommets[currNode]->pred;
    }
    printf(" %d\n", currNode);
}

void displayGrapheInfo(graphe_t *graphe)
{
    arc_t *tmpArc;

    printf("Ordre: %d\nTaille: %d\nOriente: ", graphe->ordre, graphe->taille);

    for (int i = 0; i < graphe->ordre; i++) {
        printf("Sommet %d:\n", i);
        tmpArc = graphe->sommets[i]->arc;
        while (tmpArc != NULL) {
            printf("%d ", tmpArc->sommet->id);
            tmpArc = tmpArc->next;
        }
        printf("\n");
    }
}