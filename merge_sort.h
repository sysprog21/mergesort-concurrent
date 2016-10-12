#ifndef _MERGE_SORT_H_
#define _MERGE_SORT_H_

#include "list.h"

llist_t *sort_n_merge(llist_t *a, llist_t *b);
llist_t *split_n_merge(llist_t *list);
llist_t *merge_sort(llist_t *list);

#endif
