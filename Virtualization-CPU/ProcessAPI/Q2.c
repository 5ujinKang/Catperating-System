/* PROBLEM : 
  Write a program that opens a file (with the open() system call) and then calls fork() to create a new process.
  Can both the child and parent access the file descriptor returned by open()?
  What happens when they are writing to the file concurrently, i.e., at the same time?
*/

/* ================================================================================================
LOGICAL TRY ANSWER
  1. Can both the child and parent access the file descriptor returned by open()? : 
      YES. Since they share the same setting from the point that fork() called. 
      If fork() called after open(), they will share same file descriptor returned by open().
      
  2. What happens when they are writing to the file concurrently, i.e., at the same time? :
      I think there might not be a problem about crashing, since no processes can work at the same time (precisely).
      However, I guess there could be some mixed-ordered write executions depends on execution order of process.

      which means, We can't assure that writing instruction will happens
      - 1) Child -> parent
      - 2) Parent -> Child
      - 3) Child -> parent -> child -> parent ... whatever (with context change, if there are lots of instructions)

      unless we use Wait() function in parent. => if we do so, we can stick to 1) Child->parent always.
*/

/*
  AFTER CODING TEST : ABOVE SEEMS TRUE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])        {
        printf("Main func / pid : %d \n", (int) getpid());

        // open file
        close(STDOUT_FILENO);
        open("hw.txt", O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);

        int rc = fork();

        if(rc < 0){
                fprintf(stderr, "fork failed\n");
                exit(1);
        }
        else if(rc == 0){
                printf("Child process / pid : %d\n", (int) getpid());
                printf("This is from Child\n");
        }
        else{
                int rc_wait = wait(NULL); // if I add this one, under instructions for parents aren't proceed until child process exit.
                printf("Parent process / pid : %d, childPID : %d\n", (int)getpid(), rc);
                printf("This is from Parent\n");
        }

        return 0;
}
