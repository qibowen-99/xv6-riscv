#include "thread.h"
#include <stdlib.h>
#define PGSIZE 4096

int thread_create(void *(start_routine)(void*), void *arg) {
    thread_t *t = malloc(sizeof(thread_t));
    if (t == NULL)
        return -1;

    t->start_routine = start_routine;
    t->arg = arg;
    t->stack = malloc(PGSIZE);
    if(t->stack == NULL) {
        free(t);
        return -1;
    }

    int tid = clone(t);
    if(tid < 0) {
        free(t->stack);
        free(t);
        return -1;
    }

    return tid;
}


void lock_init(lock_t* lock){
    lock->locked = 0;
}

void lock_acquire(lock_t* lock){
    while(__sync_lock_test_and_set(&lock->locked, 1) != 0)
        ;
}

void lock_release(lock_t* lock){
    __sync_lock_release(&lock->locked);
}