/*
2. Measure Context Switch Overhead
Create two processes that communicate via two UNIX pipes (or sockets), 
using a ping-pong pattern: one process writes to a pipe and waits to read; the other reads and then writes back.

This triggers real context switches between the two processes.

Measure the total round-trip communication time, then divide by the number of context switches to get an average switch cost.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>


int main(int argc, char *argv[])        {
        printf("Main func / pid : %d \n", (int) getpid());

        int pipefd_PC[2]; // make pipe Parent->Child write, C->P read
        int pipefd_CP[2]; // pipe C->P write, P->C read

        if(pipe(pipefd_PC) == -1){
                fprintf(stderr, "pipePC failed\n");
                exit(1);
        }

        if(pipe(pipefd_CP) == -1){
                fprintf(stderr, "pipeCP failed\n");
                exit(1);
        }

        int bufSz = 128;
        char buf_P[bufSz];
        char buf_C[bufSz];

        long seconds;
        long microseconds;
        double total_time;
  
        int context_switch = 2;

        struct timeval start, end;
      
        int rc = fork();

        if(rc < 0){
                fprintf(stderr, "fork failed\n");
                exit(1);
        }
        else if(rc == 0){ // child
                close(pipefd_CP[0]); // Need to clog read side of pipefd A->B
                close(pipefd_PC[1]); // Need to clog write side of pipefd B->A

                read(pipefd_PC[0], buf_C, bufSz); // Context Switch (blocked until receive writing)
                write(pipefd_CP[1], "This is from Child", bufSz);
          
                close(pipefd_PC[0]);
                close(pipefd_CP[1]);
        }
        else{ // parent
                printf("Parent process / pid : %d, childPID : %d\n", (int)getpid(), rc);

                close(pipefd_PC[0]);
                close(pipefd_CP[1]);

                gettimeofday(&start, NULL);
          
                write(pipefd_PC[1], "This is from Parent", bufSz);
                read(pipefd_CP[0], buf_P, bufSz); // Context Switch (blocked until receive writing)

                gettimeofday(&end, NULL);
          
                printf("This is parent Print Section ==== \n");
                printf("%s\n", buf_P);

                close(pipefd_PC[1]);
                close(pipefd_CP[0]);

                seconds = end.tv_sec - start.tv_sec;
                microseconds = end.tv_usec - start.tv_usec;
          
                total_time = seconds * 1e6 + microseconds; // convert to microseconds

                double avg_time = total_time / context_switch;

                printf("measured time in Parent (us) : %f\n\n", avg_time);
        }
                                                       
        return 0;
}


/* RESULT
waterjin@SUJIN-LAPTOP:~/ostep-code/cpu-api$ ./de1
Main func / pid : 131
Parent process / pid : 131, childPID : 132
This is parent Print Section ====
This is from Child

measured time in Parent (us) : 43.500000
waterjin@SUJIN-LAPTOP:~/ostep-code/cpu-api$ ./de1
Main func / pid : 133
Parent process / pid : 133, childPID : 134
This is parent Print Section ====
This is from Child

measured time in Parent (us) : 77.000000
waterjin@SUJIN-LAPTOP:~/ostep-code/cpu-api$ ./de1
Main func / pid : 135
Parent process / pid : 135, childPID : 136
This is parent Print Section ====
This is from Child

measured time in Parent (us) : 62.000000
*/
