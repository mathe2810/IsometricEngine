//
// Created by mathe on 08/10/2023.
//
#include "../includes/Graphes.h"

int *getWay(graphe_t *graphe, int sDep, int sFin, int *nbCheminTrouve)
{
    int currNode = sFin;
    int nbChemin=0;

    if (graphe->sommets[sFin]->dist == MAX_INT) {
        allegro_message("Le sommet final est dans une autre composante connexe");
        return 0;
    }

    printf("\nDistance: %d | Chemin:", graphe->sommets[sFin]->dist);

    while (currNode != sDep) {
        printf(" %d <-", currNode);
        currNode = graphe->sommets[currNode]->pred;
        nbChemin++;
    }
    nbChemin++;
    *nbCheminTrouve=nbChemin;
    printf("%d\n",currNode);
    int *pred= malloc(sizeof (int)*(nbChemin));
    currNode = sFin;
    pred[nbChemin]=currNode;
    nbChemin--;
    while (currNode != sDep) {
        pred[nbChemin]=currNode;
        currNode = graphe->sommets[currNode]->pred;
        nbChemin--;
    }
    pred[nbChemin]=currNode;
    return pred;
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