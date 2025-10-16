/*
    Next, build a version of the approximate counter. 
    Once again, measure its performance as the number of threads varies, 
    as well as the threshold. 
    
    Do the numbers match what you see in the chapter?
*/

// compile gcc -O2 -pthread -o approx_counter approx_counter.c

/////// RESULT ///////////

/* threshold : 1024(fixed), number of thread : 1 ~ 8

| Threads | Total Increments | Single Lock Time (s) | Approximate Counter Time (s) | Speedup (Single / Approx) |
| ------- | ---------------- | -------------------- | ---------------------------- | ------------------------- |
| 1       | 1,000,000        | 0.027904             | 0.026343                     | 1.06×                     |
| 2       | 2,000,000        | 0.113942             | 0.059082                     | 1.93x                     |
| 3       | 3,000,000        | 0.158891             | 0.183071                     | 0.87× (slower)            |
| 4       | 4,000,000        | 0.288089             | 0.207483                     | 1.39×                     |
| 5       | 5,000,000        | 0.431754             | 0.258494                     | 1.67×                     |
| 6       | 6,000,000        | 0.545343             | 0.291145                     | 1.87×                     |
| 7       | 7,000,000        | 0.700177             | 0.322565                     | 2.17×                     |
| 8       | 8,000,000        | 0.888051             | 0.343270                     | 2.59×                     |

 Interpretation
- For T ≥ 2, the approximate counter starts showing clear performance gains.
- At 3 threads, approximate was slightly slower
- From 4 threads onward, the approximate counter consistently outperforms the single-lock version, and the scaling improves significantly with more threads.
=> having lock per CPU improves performance in this situation.

Why it works best at 8 threads? 
    because they don't need to compete for 1 lock. each thread can have it's own lock if threads are distributed fairly.
    think about with the case of only 1 lock. => struggle gets harsh .
*/


/* 8 threads, threshold(S) variation
 Total increments: 8000000
Final counter value: 8000000
result : approximate_counter_threshold_increase.jpg

same with textbook : generally the bigger the threshold, less time
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUMCPUS 8
#define NUM_ITER 1000000

typedef struct __counter_t {
    int global;                      // global count
    pthread_mutex_t glock;          // global lock
    int local[NUMCPUS];             // per-CPU count
    pthread_mutex_t llock[NUMCPUS]; // per-CPU locks
    int threshold;                  // update frequency
} counter_t;

void init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);
    for (int i = 0; i < NUMCPUS; i++) {
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}

void update(counter_t *c, int threadID, int amt) {
    int cpu = threadID % NUMCPUS;
    pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amt;
    if (c->local[cpu] >= c->threshold) {
        pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];
        pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;
    }
    pthread_mutex_unlock(&c->llock[cpu]);
}

int get(counter_t *c) {
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val; // only approximate
}

typedef struct {
    counter_t *counter;
    int thread_id;
    int num_updates;
} thread_arg_t;

void *worker(void *arg) {
    thread_arg_t *a = (thread_arg_t *)arg;
    for (int i = 0; i < a->num_updates; i++) {
        update(a->counter, a->thread_id, 1);
    }
    return NULL;
}

double current_time() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_threads> <threshold>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int threshold = atoi(argv[2]);

    counter_t counter;
    init(&counter, threshold);

    pthread_t threads[num_threads];
    thread_arg_t args[num_threads];

    double start = current_time();

    for (int i = 0; i < num_threads; i++) {
        args[i].counter = &counter;
        args[i].thread_id = i;
        args[i].num_updates = NUM_ITER;
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Transfer remaining local counts to global
    for (int i = 0; i < NUMCPUS; i++) {
        pthread_mutex_lock(&counter.llock[i]);
        if (counter.local[i] > 0) {
            pthread_mutex_lock(&counter.glock);
            counter.global += counter.local[i];
            pthread_mutex_unlock(&counter.glock);
            counter.local[i] = 0;
        }
        pthread_mutex_unlock(&counter.llock[i]);
    }

    double end = current_time();

    int expected = num_threads * NUM_ITER;
    int final_count = get(&counter);

    printf("Threads: %d, Threshold: %d\n", num_threads, threshold);
    printf("Total increments: %d\n", expected);
    printf("Final counter value: %d\n", final_count);
    printf("Elapsed time: %.6f seconds\n", end - start);

    return 0;
}
