//
// Created by mathe on 08/10/2023.
//

#ifndef ISOMETRICENGINE_STRUCT_H
#define ISOMETRICENGINE_STRUCT_H
#include <stdbool.h>

#define MAX_INT 2147483647

typedef struct maillon_s {
    int valeur;
    struct maillon_s *next;
} maillon_t;

typedef struct file_s {
    maillon_t *start;
    maillon_t *end;
} file_t;

typedef struct arc_s {
    struct arc_s *next;
    int w;
    struct sommet_s *sommet;
} arc_t;

typedef struct sommet_s {
    int id;
    int couleur;
    int pred;
    int dist;
    int compoConnex;
    arc_t *arc;
} sommet_t;

typedef struct graphe_s {
    int taille;
    int ordre;
    bool orientation;
    sommet_t **sommets;
} graphe_t;

#endif //ISOMETRICENGINE_STRUCT_H
