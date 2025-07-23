#pragma once

#include "core/quad.h"
#include <stddef.h>


typedef struct pxluv_poly_node {
    pxluv_quad_t data;
    struct pxluv_poly_node* next;
} pxluv_poly_node_t;

typedef struct pxluv_poly_list {
    pxluv_poly_node_t* begin;
} pxluv_poly_list_t;

#define PXLUV_POLY_LIST_FOREACH(list, current) \
    for (current = list->begin; current != NULL; current = current->next)

pxluv_poly_list_t pxluv_poly_list_create();

void pxluv_poly_list_add(pxluv_poly_list_t* list, pxluv_quad_t data);

void pxluv_poly_list_remove_index(pxluv_poly_list_t* list, int i);

int pxluv_poly_list_size(pxluv_poly_list_t* list);