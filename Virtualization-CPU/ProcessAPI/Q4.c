/* QUESTION : 
  Write a program that calls fork() and then calls some form of exec() to run the program /bin/ls.
  See if you can try all of the variants of exec(), 
  including (on Linux) execl(), execle(), execlp(), execv(), execvp(), and execvpe().
    : Yes we can
    
  Why do you think there are so many variants of the same basic call?
    : The reason there are multiple variants of the exec() system call is to allow fine-grained control 
      over how a new program is executed, depending on different needs.

      If you want to pass arguments one by one → use execl
      If you want to pass arguments as an array → use execv
      If you need to set a new environment → use execle or execvpe
      If you want the system to search for the executable in the PATH environment variable → use execlp, execvp, or execvpe

      In short, these different versions make it easier for programmers to choose the most convenient and appropriate way 
      to launch a new program, depending on their specific requirements.
*/

#define _GNU_SOURCE
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
                
                // exec section ====================================
                execl("/bin/ls","/bin/ls", NULL); // 1. execl version

                /* 2. execle version
                char *envp[] = { NULL };
                execle("/bin/ls", "/bin/ls", (char *)NULL, envp);
                */

                /* 3. execlp version
                execlp("ls", "ls", (char *)NULL);
                */

                /* 4. execv version
                char *argv[] = { "/bin/ls", NULL };
                execv("/bin/ls", argv);
                */

                /* 5. execvp version
                char *argv[] = { "ls", NULL };
                execvp("ls", argv);
                */

                /* 6. execvpe version
                char *argv[] = { "ls", NULL };
                char *envp[] = { NULL };
                execvpe("ls", argv, envp);
                */
            
                // =================================================

          
                execl("/bin/ls","/bin/ls", NULL);
                printf("This shouldn't be printed\n");
        }
        else{
                int rc_wait = wait(NULL);
                printf("Parent process / pid : %d, childPID : %d\n", (int)getpid(), rc);
        }

        return 0;
}
