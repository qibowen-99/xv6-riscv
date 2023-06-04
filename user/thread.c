#include "thread.h"
#define PGSIZE 4096


static void thread_start(void* (*start_routine)(void*), void *arg){
    start_routine(arg);
    exit(0);
}

int thread_create(thread_t *t, void *(start_routine)(void*), void *arg){
    t->start_routine = start_routine;
    t->arg = arg;
    t->stack = malloc(PGSIZE);
    if(t->stack == 0)
        return -1;

    int tid = clone(thread_start, t->stack+PGSIZE, t);
    if(tid < 0){
        free(t->stack);
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