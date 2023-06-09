#include "thread.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
//#include <stdlib.h>
#define PGSIZE 4096

int thread_create(void *(start_routine)(void*), void *arg) {
    thread_t *t = malloc(sizeof(thread_t));
    if (t == 0)
        return -1;
    printf("start routine loc: %d\n",(uint64)start_routine);
    t->start_routine = start_routine;
    t->arg = arg;
    t->stack = malloc(PGSIZE);
    if(t->stack == 0) {
        free(t);
        return -1;
    }

    printf("test\n");
    int tid = clone(t);
    printf("End\n");
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
