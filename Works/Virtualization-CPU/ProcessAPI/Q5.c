/* Problem
  Write a program that uses wait() to wait for the child process to finish in the parent.
  What does wait() return? : It returns child's PID => Correct

  What happens if you use wait() in the child? : Return 0 => return -1
*/

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
                int child_rc = wait(NULL);
                printf("\nreturn value from wait() in Child : %d\n", child_rc);
                printf("Child process / pid : %d\n", (int) getpid());
        }
        else{
                int rc_wait = wait(NULL);
                printf("\nreturn value from wait(): %d\n", rc_wait);
                printf("Parent process / pid : %d, childPID : %d\n", (int)getpid(), rc);
        }

        return 0;
}
