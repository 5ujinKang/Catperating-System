/*
1. Measure System Call Overhead
Write a small program that repeatedly invokes a simple system call, such as a 0-byte read().

Use gettimeofday() to time the total duration of the repeated calls.

Divide the total time by the number of iterations to get the average cost of a system call.

To assess timer accuracy, measure back-to-back calls to gettimeofday(). If it’s not precise enough, consider using rdtsc.

*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>

#define ITERATIONS 10000000



int main(int argc, char *argv[]){

  int fd = open("/dev/null", O_RDONLY);  // Open a dummy file

  if(fd < 0){
    fprintf(stderr, "file descriptor open failed.\n");
    return 1;
  }

  struct timeval start, end;

  gettimeofday(&start, NULL);

  for(int i = 0; i < ITERATIONS; i++){ // read 0-byte for ITERATIONS times
    if(read(fd, NULL, 0) < 0){
      fprintf(stderr, "read failed\n");
      return 1;
    }
  }

  gettimeofday(&end, NULL);

  close(fd);
  
  long seconds = end.tv_sec - start.tv_sec;
  long microseconds = end.tv_usec - start.tv_usec;
  
  double total_time = seconds * 1e6 + microseconds; // convert to microseconds
  
  double avg_time = total_time / ITERATIONS;

  printf("Total iterations : %d\n", ITERATIONS);
  printf("Total time (us) : %f\n", total_time);
  printf("Average time(us) : %f\n", avg_time);
  
  return 0;
}

/*
result #1.
Total iterations : 10000000
Total time (us) : 886821.000000
Average time(us) : 0.088682

result #2.
Total iterations : 10000000
Total time (us) : 847499.000000
Average time(us) : 0.084750

result #3.
Total iterations : 10000000
Total time (us) : 909615.000000
Average time(us) : 0.090962
*/
