#include <stdlib.h>
#include <stdio.h>

#include "list.h"

/**
 * @brief Create a new node with data _val_ and set the next node to _net_
 * @param val Specifiy the data to assign to the new node
 * @param next Pointer to the next node
 * @return Pointer to the created new node
 */
static node_t *node_new(val_t val, node_t *next)
{
    /* allocate node */
    node_t *node = malloc(sizeof(node_t));
    node->data = val;
    node->next = next;
    return node;
}

/**
 * @brief Initialize the linked list.
 *
 * The function will allocate a block of memory for _llist\_t_ and
 * initialize its members _head_ to NULL and _size_ to 0.
 *
 * @return Pointer to the allocated _llist\_t_
 */
llist_t *list_new()
{
    /* allocate list */
    llist_t *list = malloc(sizeof(llist_t));
    list->head = NULL;
    list->size = 0;
    return list;
}

/**
 * @brief Insert a new node with the given value val at the head of the _list_
 * @param list The target linked list
 * @param val Specify the value
 * @return The final size of the linked list
 */
int list_add(llist_t *list, val_t val)
{
    node_t *e = node_new(val, NULL);
    e->next = list->head;
    list->head = e;
    list->size++;
    return list->size;
}

/**
 * @brief Get the node specified by index
 * If the index is out of range, it will return NULL.
 *
 * @param list The target linked list
 * @param idx Specify the index of the node in the _list_
 * @return The node at index _idx_.
 */
node_t *list_get(llist_t *list, uint32_t idx)
{
    if (!(idx < list->size))
        return NULL;
    node_t *head = list->head;
    while (idx--)
        head = head->next;
    return head;
}

/**
 * @brief Display the data of all nodes in the linked list
 * @param list The target linked list
 */
void list_print(llist_t *list)
{
    node_t *cur = list->head;
    /* FIXME: we have to validate the sorted results in advance. */
    printf("\nsorted results:\n");
    while (cur) {
        printf("[%ld] ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}
