/* QUESTION : 
  Write a program that creates a child process, and then in the child closes standard output(STDOUT_FILENO).
  What happens if the child calls printf() to print some output after closing the descriptor?
*/

// PAPER TRY : Don't print anything since standard output is closed. Unless open new output(ex. to file)
// Code result : above seems correct.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[])        {
        printf("Main func / pid : %d \n", (int) getpid());
        int rc = fork();

        if(rc < 0){
                fprintf(stderr, "fork failed\n");
                exit(1);
        }
        else if(rc == 0){
                close(STDOUT_FILENO);
                printf("Child process / pid : %d\n", (int) getpid());
        }
        else{
                printf("Parent process / pid : %d, childPID : %d\n", (int)getpid(), rc);
        }

        return 0;
}
