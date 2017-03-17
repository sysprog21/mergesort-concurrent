#include "threadpool.h"

/**
 * @brief Release the memory allocated in _task\_t_
 *
 * Note that you have to make sure that the resourse which
 * _task\_t->arg_ points to is no longer used after this
 * function call.
 *
 * @param the_task Pointer to the target _task\_t_
 */
int task_free(task_t *the_task)
{
    free(the_task->arg);
    free(the_task);
    return 0;
}

/**
 * @brief Create a new task.
 */
task_t *task_new(void (*func)(void *), void *arg)
{
    task_t *new_task = (task_t *)malloc(sizeof(task_t));
    new_task->func = func;
    new_task->arg = arg;
    return new_task;
}

/**
 * @brief Initialize the members in _tqueue\_t_.
 *
 * Note that this function doesn't allocate memory for the task queue.
 *
 * @param the_queue Pointer to the target _tqueue\_t_
 * @return -1 if _the\_queue_ is NULL. Otherwise, return 0.
 */
int tqueue_init(tqueue_t *the_queue)
{
    if (!the_queue)
        return -1;
    the_queue->head = NULL;
    the_queue->tail = NULL;
    pthread_mutex_init(&(the_queue->mutex), NULL);
    pthread_cond_init(&(the_queue->cond), NULL);
    the_queue->size = 0;
    the_queue->num_of_consumed = 0;
    return 0;
}

/**
 * @brief Pop a task from the task queue.
 * @param the_queue Pointer to the target task queue
 * @return The popped task. NULL if the queue is empty.
 */
task_t *tqueue_pop(tqueue_t * const the_queue)
{
    task_t *ret;
    pthread_mutex_lock(&(the_queue->mutex));
    ret = the_queue->head;
    if (ret) {
        the_queue->head = ret->next;
        ret->next = NULL;
        if (!(the_queue->head)) {
            the_queue->tail = NULL;
        }
        the_queue->size--;
        the_queue->num_of_consumed++;
    }
    pthread_mutex_unlock(&(the_queue->mutex));
    return ret;
}

/**
 * @brief Get the number of the tasks in the task queue
 * @param the_queue Pointer to the target task queue
 * @return The number of the tasks in the task queue
 */
uint32_t tqueue_size(tqueue_t * const the_queue)
{
    uint32_t ret;
    pthread_mutex_lock(&(the_queue->mutex));
    ret = the_queue->size;
    pthread_mutex_unlock(&(the_queue->mutex));
    return ret;
}

/**
 * @brief Push a task to the task queue
 * @param the_queue Pointer to the target task queue
 * @param task Pointer to the task to push
 */
int tqueue_push(tqueue_t * const the_queue, task_t *task)
{
    pthread_mutex_lock(&(the_queue->mutex));
    task->next = NULL;
    if (the_queue->tail)
        the_queue->tail->next = task;
    the_queue->tail = task;
    if (the_queue->size++ == 0)
        the_queue->head = task;
    pthread_mutex_unlock(&(the_queue->mutex));
    return 0;
}

/**
 * @brief Release the memory of the remaining tasks in the task queue and destory the mutex lock
 * @param the_queue The target task queue
 * @return The number of task consumed.
 */
uint32_t tqueue_free(tqueue_t *the_queue)
{
    uint32_t num_of_consumed = the_queue->num_of_consumed;
    task_t *cur = the_queue->head;
    while (cur) {
        the_queue->head = the_queue->head->next;
        free(cur);
        cur = the_queue->head;
    }
    pthread_mutex_destroy(&(the_queue->mutex));
    return num_of_consumed;
}

/**
 * @brief Create and initialize the thread pool
 * @param the_pool Pointer to the created thread pool
 * @param tcount The number of the threads to be created
 * @param func Pointer to the task function
 */
int tpool_init(tpool_t *the_pool, uint32_t tcount, void *(*func)(void *))
{
    the_pool->threads = (pthread_t *) malloc(sizeof(pthread_t) * tcount);
    the_pool->count = tcount;
    the_pool->queue = (tqueue_t *) malloc(sizeof(tqueue_t));
    tqueue_init(the_pool->queue);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for (uint32_t i = 0; i < tcount; ++i)
        pthread_create(&(the_pool->threads[i]), &attr, func, NULL);
    pthread_attr_destroy(&attr);
    return 0;
}

/**
 * @brief Join all the threads and release the allocated memory of thread pool
 * @param the_pool The target thread pool
 * @return The number of consumed tasks
 */
uint32_t tpool_free(tpool_t *the_pool)
{
    int num_of_consumed;
    for (uint32_t i = 0; i < the_pool->count; ++i)
        pthread_join(the_pool->threads[i], NULL);
    free(the_pool->threads);
    num_of_consumed = tqueue_free(the_pool->queue);
    return num_of_consumed;
}
