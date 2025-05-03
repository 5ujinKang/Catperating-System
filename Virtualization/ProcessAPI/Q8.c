/* QUESTION : 
  Write a program that creates two children, 
  and connects the standard output of one to the standard input of the other, using the pipe()system call.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])        {
        printf("Main func / pid : %d \n", (int) getpid());

        int pipefd[2]; // make pipe

        if(pipe(pipefd) == -1){
                fprintf(stderr, "pipe failed\n");
                exit(1);
        }


        // first child
        int rc1 = fork();

        if(rc1 < 0){
                fprintf(stderr, "first fork failed\n");
                exit(1);
        }
        else if(rc1 == 0){ // first child : TO WRITE
                printf("Child 1 process / pid : %d\n", (int) getpid());

                close(pipefd[0]); // Need to clog read side of pipefd

                dup2(pipefd[1],STDOUT_FILENO); // overwrite STDOUT of child1 process into pipefd[1] : write
                close(pipefd[1]); // no need anymore

                printf("This is from child 1 stdout\n");

        }
        else{
                printf("Parent process / pid : %d, childPID : %d\n", (int)getpid(), rc1);
        }

        int rc2 = fork();

        if(rc2 < 0){
                fprintf(stderr, "second fork failed\n");
                exit(1);
        }
        else if(rc2 == 0){ // Second child : TO READ
                printf("Child 2 process / pid : %d\n", (int)getpid());

                close(pipefd[1]); // clog read side

                dup2(pipefd[0], STDIN_FILENO); // overwrite STDIN into pipefd's read part
                close(pipefd[0]);// close since no need

                char buffer[128];                                               
                fgets(buffer, sizeof(buffer), stdin); // Read from stdin
                printf("Child 2 output : %s\n", buffer);

        }
        else {
                printf("Parent process / pid : %d, childPID : %d\n", (int)getpid(), rc2);
        }                                                       
        return 0;
}

// ================ result ====================
/*
Main func / pid : 102
Parent process / pid : 102, childPID : 103
Child 1 process / pid : 103
Parent process / pid : 102, childPID : 104
Child 2 process / pid : 104
Child 2 output : This is from child 1 stdout

*/
