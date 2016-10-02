#ifndef LLIST_H_
#define LLIST_H_

#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>

typedef intptr_t val_t;

typedef struct node {
    val_t data;
    struct node *next;
} node_t;

typedef struct llist {
    node_t *head;
    uint32_t size;
} llist_t;

llist_t *list_new();
int list_add(llist_t *the_list, val_t val);
void list_print(llist_t *the_list);
node_t *new_node(val_t val, node_t *next);
node_t *list_get(llist_t *the_list, uint32_t index);

#endif
