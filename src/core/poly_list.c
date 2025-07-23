#include "core/poly_list.h"
#include <stddef.h>
#include <stdlib.h>
#include "core/pxluv.h"

pxluv_poly_list_t pxluv_poly_list_create() {
    pxluv_poly_list_t list;
    list.begin = NULL;
    return list;
}

void pxluv_poly_list_add(pxluv_poly_list_t* list, pxluv_quad_t data) {
    pxluv_poly_node_t** current;
    for (current = &list->begin;; current = &(*current)->next) {
        if (*current == NULL) {
            *current = malloc(sizeof(pxluv_poly_node_t));
            (*current)->data = data;
            (*current)->next = NULL;
            return;
        }
    }
}

void pxluv_poly_list_remove_index(pxluv_poly_list_t* list, int i) {
    int c_i = 0;
    pxluv_poly_node_t** current;
    for (current = &list->begin; *current != NULL; current = &(*current)->next) {
        if (c_i == i) {
            pxluv_poly_node_t* to_delete = *current;
            *current = (*current)->next;
            free(to_delete);
            return;
        }

        c_i++;
    }

    pxluv_log("Couln't delete index %d, is is not in list.", i);
}

int pxluv_poly_list_size(pxluv_poly_list_t* list) {
    pxluv_poly_node_t* current;
    int count = 0;
    PXLUV_POLY_LIST_FOREACH(list, current) {
        count++;
    }

    return count;
}