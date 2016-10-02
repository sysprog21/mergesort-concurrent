#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "thread.h"
#include "list.h"

#define USAGE "usage: ./sort [thread counts] [unsorted data]\n"

struct {
    pthread_mutex_t mutex;
    int cutCount;
} tData;

struct {
    pthread_mutex_t mutex;
    llist_t *list;
} tmpList;

llist_t *the_list = NULL;
int tCount = 0, dataCount = 0, maxCut = 0;
tpool_t *pool = NULL;

llist_t *mergeList(llist_t *a, llist_t *b)
{
    llist_t *newList = list_new();
    node_t *cur = NULL;
    while (a->size && b->size) {
        llist_t *smallList = (llist_t *)
            ((intptr_t) a * (a->head->data <= b->head->data) +
             (intptr_t) b *(a->head->data > b->head->data));
        if (cur) {
            cur->next = smallList->head;
            cur = cur->next;
        } else {
            newList->head = smallList->head;
            cur = newList->head;
        }
        smallList->head = smallList->head->next;
        --smallList->size;
        ++newList->size;
        cur->next = NULL;
    }
    llist_t *remainList = (llist_t *)
        ((intptr_t) a * (a->size > 0) +
         (intptr_t) b * (b->size > 0));
    cur->next = remainList->head;
    newList->size += remainList->size;
    free(a);
    free(b);
    return newList;
}

llist_t *mergeSort(llist_t *list)
{
    if (list->size < 2) {
        return list;
    }
    int mid = list->size / 2;
    llist_t *newlist = list_new();
    newlist->head = list_get(list, mid);
    newlist->size = list->size - mid;
    list_get(list, mid-1)->next = NULL;
    list->size = mid;
    return mergeList(mergeSort(list), mergeSort(newlist));
}

void merge(void *data)
{
    llist_t *list = (llist_t *)data;
    if (list->size < dataCount) {
        pthread_mutex_lock(&(tData.mutex));
        llist_t * tmpLocal = tmpList.list;
        if(!tmpLocal) {
            tmpList.list = list;
            pthread_mutex_unlock(&(tData.mutex));
        } else {
            tmpList.list = NULL;
            pthread_mutex_unlock(&(tData.mutex));
            task_t *newTask = (task_t *)malloc(sizeof(task_t));
            newTask->function = merge;
            newTask->arg = mergeList(list, tmpLocal);
            tqueue_push(pool->queue, newTask);
        }
    } else {
        the_list = list;
        task_t *newTask = (task_t *)malloc(sizeof(task_t));
        newTask->function = NULL;
        tqueue_push(pool->queue, newTask);
        list_print(list);
    }
}

void cut(void *data)
{
    llist_t *list = (llist_t *)data;
    pthread_mutex_lock(&(tData.mutex));
    int cutLocal = tData.cutCount;
    if(list->size > 1 && cutLocal < maxCut) {
        ++tData.cutCount;
        pthread_mutex_unlock(&(tData.mutex));
        // Cut list
        int mid = list->size / 2;
        llist_t *newlist = list_new();
        newlist->head = list_get(list, mid);
        newlist->size = list->size - mid;
        list_get(list, mid-1)->next = NULL;
        list->size = mid;
        // Make new task
        task_t *newTask = (task_t *)malloc(sizeof(task_t));
        newTask->function = cut;
        newTask->arg = newlist;
        tqueue_push(pool->queue, newTask);
        newTask = (task_t *)malloc(sizeof(task_t));
        newTask->function = cut;
        newTask->arg = list;
        tqueue_push(pool->queue, newTask);
    } else {
        pthread_mutex_unlock(&(tData.mutex));
        merge(mergeSort(list));
    }
}

void *tRun(void *data)
{
    // Run
    task_t *curTask = NULL;
    int running = 1;
    while (running) {
        curTask = tqueue_pop(pool->queue);
        if(curTask) {
            if(!curTask->function) {
                tqueue_push(pool->queue, curTask);
                break;
            } else {
                curTask->function(curTask->arg);
                free(curTask);
            }
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    if(argc < 3) {
        printf(USAGE);
        return -1;
    }
    tCount = atoi(argv[1]);
    dataCount = atoi(argv[2]);
    maxCut = tCount*(tCount <= dataCount) + dataCount*(tCount > dataCount) - 1;
    // Read data
    the_list = list_new();
    for(int i=0; i<dataCount; ++i) {
        long int data;
        scanf("%ld", &data);
        list_add(the_list, data);
    }
    // Init & run thread pool
    pthread_mutex_init(&(tData.mutex), NULL);
    tData.cutCount = 0;
    tmpList.list = NULL;
    pool = (tpool_t *) malloc(sizeof(tpool_t));
    tpool_init(pool, tCount, tRun);
    // Set first task
    task_t *newTask = (task_t *)malloc(sizeof(task_t));
    newTask->function = cut;
    newTask->arg = the_list;
    tqueue_push(pool->queue, newTask);
    // Free thread pool
    tpool_free(pool);
    return 0;
}
