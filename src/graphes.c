//
// Created by mathe on 08/10/2023.
//
#include "../includes/Graphes.h"

arc_t *addArc(arc_t *arc, sommet_t *nextNode, int weight)
{
    arc_t *tmpArc = arc;

    if (arc == NULL){
        arc = malloc(sizeof(arc_t));
        handleMalloc(arc);
        arc->sommet = nextNode;
        arc->next= NULL;
        arc->w = weight;
        return arc;
    }

    while (tmpArc->next != NULL)
        tmpArc = tmpArc->next;

    tmpArc->next = malloc(sizeof(arc_t));
    handleMalloc(tmpArc->next);
    tmpArc->next->next = NULL;
    tmpArc->next->w = weight;
    tmpArc->next->sommet = nextNode;
    return arc;
}

void initGraphe(graphe_t *graphe, FILE *fp)
{
    int nextNode, currentNode, weight;

    for (int i = 0; i < graphe->ordre; i++) {
        graphe->sommets[i]->arc = NULL;
        graphe->sommets[i]->dist = MAX_INT;
        graphe->sommets[i]->pred = -1;
    }

    for (int i = 0; i < graphe->taille; i++) {
        fscanf(fp, "%d%d%d", &currentNode, &nextNode, &weight);
        graphe->sommets[currentNode]->arc = addArc(graphe->sommets[currentNode]->arc, graphe->sommets[nextNode], weight);
        graphe->sommets[nextNode]->arc = addArc(graphe->sommets[nextNode]->arc, graphe->sommets[currentNode], weight);
    }
}

graphe_t *loadGraphe()
{
    FILE *fp= fopen("../djikstra.txt","r");
    graphe_t *graphe = malloc(sizeof(graphe_t));

    handleMalloc(graphe);
    fscanf(fp, "%d", &graphe->ordre);
    graphe->sommets = malloc(sizeof(sommet_t *) * graphe->ordre);
    handleMalloc(graphe->sommets);
    for (int i = 0; i < graphe->ordre; i++) {
        graphe->sommets[i] = malloc(sizeof(sommet_t));
        handleMalloc(graphe->sommets[i]);
        graphe->sommets[i]->couleur = 0;
        graphe->sommets[i]->compoConnex = 0;
        fscanf(fp, "%d", &graphe->sommets[i]->id);
    }
    fscanf(fp, "%d", &graphe->taille);

    initGraphe(graphe, fp);

    fclose(fp);

    return graphe;
}

void freeGraphe(graphe_t *graphe)
{
    arc_t *tmpArc, *tmpArc2;

    for (int i = 0; i < graphe->ordre; i++) {
        tmpArc = graphe->sommets[i]->arc;
        while (tmpArc != NULL)  {
            tmpArc2 = tmpArc->next;
            free(tmpArc);
            tmpArc = tmpArc2;
        }
        free(graphe->sommets[i]);
    }
    free(graphe->sommets);
    free(graphe);
}
