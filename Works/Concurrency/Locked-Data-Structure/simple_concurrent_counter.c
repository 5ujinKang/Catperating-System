/*
    build a simple concurrent counter and measure how long it takes to increment the counter 
    many times as the number of threads increases. 

    How many CPUs are available on the system you are using? : 8
    
    Does this number impact your measurements at all?
    : I initially guessed 
        the execution time would remain similar until I used 8 threads (equal to my CPU core count), 
        and would increase significantly after that.
    
    But that wasn’t the case. 
    Since the program uses a critical section (protected by a mutex), 
    the updates to the counter cannot run in parallel — each thread must wait for the lock to be available.

    So, does the number of CPUs matter here?
    I don’t think so 
        : because what each thread does is almost only incrementing the shared counter, 
            which is protected by a lock. The work done outside the critical section is negligible.
            That means multiple CPUs can’t help much — the lock is the bottleneck, not the computation itself.
*/


/* RESULTS : N threads => Nx times <similar>


    Threads: 1, Total increments: 1000000, Final counter value: 1000000
    Elapsed time: 0.027904 seconds

    Threads: 2, Total increments: 2000000, Final counter value: 2000000
    Elapsed time: 0.113942 seconds

    Threads: 3, Total increments: 3000000, Final counter value: 3000000
    Elapsed time: 0.158891 seconds

    Threads: 4, Total increments: 4000000, Final counter value: 4000000
    Elapsed time: 0.288089 seconds

    Threads: 5, Total increments: 5000000, Final counter value: 5000000
    Elapsed time: 0.431754 seconds

    Threads: 6, Total increments: 6000000, Final counter value: 6000000
    Elapsed time: 0.545343 seconds

    Threads: 7, Total increments: 7000000, Final counter value: 7000000
    Elapsed time: 0.700177 seconds

    Threads: 8, Total increments: 8000000, Final counter value: 8000000
    Elapsed time: 0.888051 seconds
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Define the counter struct
typedef struct __counter_t {
    int value;
    pthread_mutex_t lock;
} counter_t;

// Initialize the counter and mutex
void init(counter_t *c) {
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

// Increment the counter safely
void increment(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

// Decrement the counter safely
void decrement(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value--;
    pthread_mutex_unlock(&c->lock);
}

// Get the current value of the counter safely
int get(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    int rc = c->value;
    pthread_mutex_unlock(&c->lock);
    return rc;
}

#define INCREMENTS 1000000

typedef struct {
    counter_t *counter;
    int increments;
} thread_arg_t;

void *worker(void *arg) {
    thread_arg_t *targ = (thread_arg_t *)arg;
    for (int i = 0; i < targ->increments; i++) {
        increment(targ->counter);
    }
    return NULL;
}

long time_diff_ns(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int max_threads = atoi(argv[1]);
    if (max_threads <= 0) {
        printf("Number of threads must be positive\n");
        return 1;
    }

    int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Number of CPUs available: %d\n", num_cpus);

    counter_t counter;
    init(&counter);

    pthread_t *threads = malloc(sizeof(pthread_t) * max_threads);
    thread_arg_t arg = { .counter = &counter, .increments = INCREMENTS };

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < max_threads; i++) {
        pthread_create(&threads[i], NULL, worker, &arg);
    }
    for (int i = 0; i < max_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    long elapsed_ns = time_diff_ns(start, end);
    printf("Threads: %d, Total increments: %d, Final counter value: %d\n",
           max_threads, max_threads * INCREMENTS, get(&counter));
    printf("Elapsed time: %.6f seconds\n", elapsed_ns / 1e9);

    free(threads);
    return 0;
}