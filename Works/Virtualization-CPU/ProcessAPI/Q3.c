/* QUESTION : 
  Write another program using fork().
  The child process should print "Hello"; the parent process should print "goodbye".
  You should try to ensure that the child process always prints first;
  can you do this without calling wait() in the parent?
*/

/* LOGICAL TRY : 
  Maybe we can just use sleep() function before print "goodbye" in parent process.
  In that way, we can delay parent write exetucion, so that allow child process always prints first.
*/

/* AFTER CODING TEST
  : ABOVE SEEMS CORRECT.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])        {
        printf("Main func / pid : %d \n", (int) getpid());

        int rc = fork();

        if(rc < 0){
                fprintf(stderr, "fork failed\n");
                exit(1);
        }
        else if(rc == 0){
                printf("Child process / pid : %d\n", (int) getpid());
                printf("Hello\n");
        }
        else{
                sleep(3);
                printf("Parent process / pid : %d, childPID : %d\n", (int)getpid(), rc);
                printf("goodbye\n");
        }

        return 0;
}
