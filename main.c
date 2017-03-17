#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "threadpool.h"
#include "list.h"
#include "merge_sort.h"

#define USAGE "usage: ./sort [thread_count] [input_file]\n"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

struct {
    pthread_mutex_t mutex;
} data_context;

static llist_t *tmp_list;
static llist_t *the_list = NULL;

static int thread_count = 0, data_count = 0, max_cut = 0;
static tpool_t *pool = NULL;

void merge_thread_lists(void *data)
{
    llist_t *_list = (llist_t *) data;
    if (_list->size < (uint32_t) data_count) {
        pthread_mutex_lock(&(data_context.mutex));
        llist_t *_t = tmp_list;
        if (!_t) {
            tmp_list = _list;
            pthread_mutex_unlock(&(data_context.mutex));
        } else {
            /*
             * If there is a local list left by other thread,
             * pick it and create a task to merge the picked list
             * and its own local list.
             */
            tmp_list = NULL;
            pthread_mutex_unlock(&(data_context.mutex));
            tqueue_push(pool->queue, task_new(merge_thread_lists, sort_n_merge(_list, _t)));
        }
    } else {
        /*
         * All local lists are merged, push a termination task to task queue.
         */
        the_list = _list;
        tqueue_push(pool->queue, task_new(NULL, NULL));
    }
}

void sort_local_list(void *data)
{
    llist_t *local_list = (llist_t *) data;
    merge_thread_lists(merge_sort(local_list));
}

void cut_local_list(void *data)
{
    llist_t *list = (llist_t *) data, *local_list;
    node_t *head, *tail;
    int local_size = data_count / max_cut;

    head = list->head;
    for (int i = 0; i < max_cut - 1; ++i) {
        /* Create local list container */
        local_list = list_new();
        local_list->head = head;
        local_list->size = local_size;
        /* Cut the local list */
        tail = list_get(local_list, local_size - 1);
        head = tail->next;
        tail->next = NULL;
        /* Create new task */
        tqueue_push(pool->queue, task_new(sort_local_list, local_list));
    }
    /* The last takes the rest. */
    local_list = list_new();
    local_list->head = head;
    local_list->size = list->size - local_size * (max_cut - 1);
    tqueue_push(pool->queue, task_new(sort_local_list, local_list));
}

static void *task_run(void *data __attribute__ ((__unused__)))
{
    while (1) {
        task_t *_task = tqueue_pop(pool->queue);
        if (_task) {
            if (!_task->func) {
                tqueue_push(pool->queue, _task);
                break;
            } else {
                _task->func(_task->arg);
                free(_task);
            }
        }
    }
    pthread_exit(NULL);
}

static uint32_t build_list_from_file(llist_t *_list, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    char buffer[16];

    while (fgets(buffer, 16, fp) != NULL) {
        list_add(_list, atol(buffer));
    }

    fclose(fp);
    return _list->size;
}

int main(int argc, char const *argv[])
{
    if (argc < 3) {
        printf(USAGE);
        return -1;
    }
    thread_count = atoi(argv[1]);

    /* Read data */
    the_list = list_new();
    data_count = build_list_from_file(the_list, argv[2]);

    max_cut = MIN(thread_count, data_count);

    /* initialize tasks inside thread pool */
    pthread_mutex_init(&(data_context.mutex), NULL);
    tmp_list = NULL;
    pool = (tpool_t *) malloc(sizeof(tpool_t));
    tpool_init(pool, thread_count, task_run);

    struct timeval start, end;
    uint32_t consumed_tasks;
    double duration;
    /* Start when the first task launches. */
    gettimeofday(&start, NULL);

    /* launch the first task */
    tqueue_push(pool->queue, task_new(cut_local_list, the_list));

    /* release thread pool */
    consumed_tasks = tpool_free(pool);

    gettimeofday(&end, NULL);

    /* Report */
    duration = (end.tv_sec - start.tv_sec) * 1000 +
               (double)(end.tv_usec - start.tv_usec) / 1000.0f;
    printf("#Total_tasks_consumed: %d\n", consumed_tasks);
    printf("#Elapsed_time: %.3lf ms\n", duration);
    printf("#Throughput: %d (per sec)\n", (uint32_t)(consumed_tasks * 1000 / duration));

    /* Output sorted result */
    list_print(the_list);

    return 0;
}
