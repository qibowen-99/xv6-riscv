#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/thread.h"


lock_t lock;
int n_threads, n_passes, cur_turn, cur_pass;
void* thread_fn(void *arg)
{
    int thread_id = (uint64)arg;
    int done = 0;
    while (!done) {
        lock_acquire(&lock);
	printf("lock acquired\n");
        if (cur_pass >= n_passes) done = 1;
	
        else if (cur_turn == thread_id) {
            cur_turn = (cur_turn + 1) % n_threads;
            printf("Round %d: thread %d is passing the token to thread %d\n",
                ++cur_pass, thread_id, cur_turn);
        }
        lock_release(&lock);
	printf("lock released\n");
        sleep(0);
    }
    return 0;
}
int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s [N_PASSES] [N_THREADS]\n", argv[0]);
        exit(-1);
    }
    n_passes = atoi(argv[1]);
    n_threads = atoi(argv[2]);
    cur_turn = 0;
    cur_pass = 0;
    //printf("making lock\n");
    lock_init(&lock);
    //printf("lock made\n");
    for (int i = 0; i < n_threads; i++) {
	//printf("making threads\n");
        thread_create(thread_fn, (void*)(uint64)i);
	//printf("made a thread\n");
    }
  //  printf("gonna wait\n");
    for (int i = 0; i < n_threads; i++) {
//	    printf("waiting...\n");
        wait(0);
//	printf("done waiting\n");
    }
    printf("Frisbee simulation has finished, %d rounds played in total\n", n_passes);
    exit(0);
}
