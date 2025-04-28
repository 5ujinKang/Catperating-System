/* Question
  Write a slight modification of the previous program, this time using waitpid() instead of wait().
  When would waitpid() be useful?
  : If a process have several childs, it's easier to control with waitpid since you can select child's pid to wait.
    wait() : wait for any one child process to exit.
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
                printf("Child process / pid : %d\n", (int) getpid());
        }
        else{
                int rc_wait = waitpid((int)getpid() +1, NULL, 0);
                // int rc_wait = wait(NULL);
                printf("\nreturn value from wait(): %d\n", rc_wait);
                printf("Parent process / pid : %d, childPID : %d\n", (int)getpid(), rc);
        }

        return 0;
}
