#include "list.h"

node_t *new_node(val_t val, node_t *next)
{
    /* allocate node */
    node_t *node = malloc(sizeof(node_t));
    node->data = val;
    node->next = next;
    return node;
}

llist_t *list_new()
{
    /* allocate list */
    llist_t *the_list = malloc(sizeof(llist_t));
    the_list->head = NULL;
    the_list->size = 0;
    return the_list;
}

/*
 * list_add inserts a new node with the given value val in the list
 * (if the value was absent) or does nothing (if the value is already present).
 */
int list_add(llist_t *the_list, val_t val)
{
    node_t *new_elem = new_node(val, NULL);
    new_elem->next = the_list->head;
    the_list->head = new_elem;
    the_list->size++;
    return 0;
}

/* NEW
 * get the node specify by index
 * if the index is out of range, it will return NULL
 */
node_t *list_get(llist_t *the_list, uint32_t index)
{
    if (index > the_list->size)
        return NULL;
    node_t *head = the_list->head;
    while (index--)
        head = head->next;
    return head;
}

void list_print(llist_t *the_list)
{
    node_t *cur = the_list->head;
    while (cur) {
        printf("%ld ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}
