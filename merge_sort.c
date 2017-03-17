#include <stdlib.h>

#include "merge_sort.h"

/**
 * @brief Sort and merge the two lists _a_ and _b_.
 */
llist_t *sort_n_merge(llist_t *a, llist_t *b)
{
    llist_t *_list = list_new();
    node_t *current = NULL;
    while (a->size && b->size) {
        /* Choose the linked list whose data of first node is small. */
        llist_t *small = (llist_t *)
                         ((intptr_t) a * (a->head->data <= b->head->data) +
                          (intptr_t) b * (a->head->data > b->head->data));
        /* Extract first node of selected list and put to the new list. */
        if (current) {
            current->next = small->head;
            current = current->next;
        } else {
            _list->head = small->head;
            current = _list->head;
        }
        small->head = small->head->next;
        --small->size;
        ++_list->size;
        current->next = NULL;
    }

    /* Append the remaining nodes */
    llist_t *remaining = (llist_t *) ((intptr_t) a * (a->size > 0) +
                                      (intptr_t) b * (b->size > 0));
    if (current) current->next = remaining->head;
    _list->size += remaining->size;
    free(a);
    free(b);
    return _list;
}

/**
 * @brief Split the list and merge later.
 */
llist_t *split_n_merge(llist_t *list)
{
    if (list->size < 2)
        return list;
    int mid = list->size / 2;
    llist_t *left = list;
    llist_t *right = list_new();
    right->head = list_get(list, mid);
    right->size = list->size - mid;
    list_get(list, mid - 1)->next = NULL;
    left->size = mid;
    return sort_n_merge(split_n_merge(left), split_n_merge(right));
}

llist_t *merge_sort(llist_t *list)
{
    return split_n_merge(list);
}
