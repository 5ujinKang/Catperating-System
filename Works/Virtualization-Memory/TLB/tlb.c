// Questions 

/* 1. 
    For timing, you’ll need to use a timer (e.g., gettimeofday()). How precise is such a timer? 
        : accuracy isn’t perfect — noise, syscall overhead, and OS scheduling mean 
        it’s not always exactly right at the sub-microsecond level.

    How long does an operation have to take in order for you to time it precisely? 
    (this will help determine how many times, 
    in a loop, you’ll have to repeat a page access in order to time it successfully)
        : one page access is tiny. a single memory access can be 5–100 ns. 
        That’s much smaller than gettimeofday() can reliably measure directly.
        So measuring a single operation directly is pointless — the timer can’t see it.

    Repeat the operation many times inside a loop to accumulate a measurable time.
    want the total time to be at least a few milliseconds to make the error negligible.

    Example: Suppose timer noise is ±1 µs. If you run your test for 10,000 µs (10 ms), that’s 0.01% relative error — acceptable.

    If each access is ~20 ns, you need about 500,000 accesses to reach 10 ms.
*/


/* 2. Write the program, called tlb.c, that can roughly measure the cost of accessing each page. 
      Inputs to the program should be: the number of pages to touch and the number of trials.
      ex : gcc -O2 -o tlb tlb.c
           ./tlb 16 1000000
            This will:

            - Touch 16 pages.
            - Repeat the measurement loop 1,000,000 times.
            - Print total time and average time per page access.

      use -O2 or higher for realistic performance measurement.

      with -O0 (no optimization)
      : The compiler keeps your C code exactly as written, step by step.
      Result: You measure compiler overhead and bad code, not hardware performance.

      -O2 tells the compiler: “Optimize for speed while keeping code safe.”
      So your loop runs at hardware speeds: you see the actual TLB hit/miss cost, not artificial compiler bloat.

*/


/* 3.
    varying the number of pages accessed from 1 up to a few thousand, 
      perhaps incrementing by a factor of two per iteration. 

    How many trials are needed to get reliable measurements?
      : 1000000. better to overshoot the number of trials to minimize noise and timer inaccuracy. 1000000
*/


/* 5.
    One thing to watch out for is compiler optimization. 
    Compilers do all sorts of clever things, including removing loops which increment values 
    that no other part of the program subsequently uses. 
        EXPLANATION : 
        for (...) {
            a[i]++;  // increments a[i], but if you never use a[i] later, it’s useless
        }
        If nothing ever reads the updated a[i] afterward, the compiler sees:

        “Why bother incrementing it? The program output will be exactly the same without it.”
        So it removes the loop entirely to make your program faster.

    How can you ensure the compiler does not remove the main loop above from your TLB size estimator?
        : add printf("%d\n", a[0]);
        after time measurement, so that compiler doesn't remove the loop.
*/


/* 6. 
    Another thing to watch out for is the fact that 
    most systems today ship with multiple CPUs, and each CPU, of course, has its own TLB hierarchy. 
    To really get good measurements, you have to run your code on just one CPU, 
    instead of letting the scheduler bounce it from one CPU to the next. 
    
    How can you do that? (hint: look up “pinning a thread” on Google for some clues) 
        : pin a process to a logical core (i got 4 physical / 8 logical core)

        - why logical, not physical ?
            The OS scheduler manages threads on logical CPUs (hardware threads), not physical cores directly.
            Each logical core has its own registers and TLB structures.
            Physical cores with hyper-threading expose multiple logical cores.

        - How can i do that? : On Linux (in C), use sched_setaffinity()

    What will happen if you don’t do this, and the code moves from one CPU to the other?
    : Without pinning, your timing results will be unreliable, inflated, and noisy 
    because CPU migration causes frequent TLB flushes or misses beyond your control.
*/


/* 7. 
    Another issue that might arise relates to initialization. 
    If you don’t initialize the array 'a' above before accessing it, the first time you access it 
    will be very expensive, due to initial access costs such as demand zeroing. (not cached.)
    
    Will this affect your code and its timing? : yes it will be longer without initialization.
        our purpose is measuring time difference due to cache miss rate between variable number of page
    
    What can you do to counterbalance these potential costs? : add initialization.
*/


// purpose of this measurement is figure out how many pages that we can save in TLB

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define PAGESIZE 4096  // 4KB page size [System.Environment]::SystemPageSize : 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_pages> <num_trials>\n", argv[0]);
        return 1;
    }

    // Pin process to logical CPU 0
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &cpuset) != 0) {
        perror("sched_setaffinity");
        return 1;
    }
    printf("Pinned to CPU 0\n");

    int num_pages = atoi(argv[1]);
    int num_trials = atoi(argv[2]);
    int jump = PAGESIZE / sizeof(int);
    int array_size = num_pages * jump;

    int *a = malloc(array_size * sizeof(int));
    if (!a) {
        perror("malloc");
        return 1;
    }

    // Initialize one element per page
    for (int i = 0; i < array_size; i += jump) {
        a[i] = 1;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int trial = 0; trial < num_trials; trial++) {
        for (int i = 0; i < array_size; i += jump) {
            a[i]++;
        }
    }

    gettimeofday(&end, NULL);

    double elapsed = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
    double total_accesses = (double)num_pages * num_trials;
    double time_per_access = elapsed / total_accesses;

    printf("Total time: %.2f us\n", elapsed);
    printf("Time per access: %.2f ns\n", time_per_access * 1000);

    // Prevent compiler optimization
    printf("Ignore this value: %d\n", a[0]);

    free(a);
    return 0;
}


/* 4. 
    Next, graph the results, making a graph that looks similar to the one above. 
    Use a good tool like ploticus or even zplot. 
    Visualization usually makes the data much easier to digest; why do you think that is?

    - tlb_results.csv
        num_pages,time_per_access_ns
        1,3.35
        2,1.66
        4,1.45 
        8,2.14
        16,3.85 
        32,4.88
        64,4.42
        128,4.70
        256,4.71
        512,4.78 
        1024,6.46 
        2048,16.27
        4096,17.52
        8192,20.09

*/