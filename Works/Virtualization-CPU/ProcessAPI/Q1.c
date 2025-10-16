/* Write a program that calls fork(). 
  Before calling fork(), have the main process access a variable (e.g., x) and set its value to something (e.g., 100).
  What value is the variable in the child process? : 100

  What happens to the variable when both the child and parent change the value of x? : changed seperately
  
  - Case 1. Both x in child and parent process are changed as 0, 50 for each
      -> x in child : 0, x in parent : 50
      
  - Case 2. only x in child is changed as 0
      -> x in child : 0, x in parent : 100
      
  - Case 3. only x in parent is changed as 50
      -> x in child : 100, x in parent : 50

  => Both parent and child start from the same main function, inherit same values at the point fork() called BUT IN DIFFERENT PHYSICAL SPACES.
     So, the change after the fork(), affects only independently. == Change in Parent <- NOT RELATED TO -> Change in Child
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[])        {
        printf("Main func / pid : %d \n", (int) getpid());        
        int x = 100;

        int rc = fork(); // child process and parent process divided.

        if(rc < 0){
                fprintf(stderr, "fork failed\n");
                exit(1);
        }
        else if(rc == 0){
                printf("Child process / pid : %d\n", (int) getpid());
                x = 0;
                printf("x in Child : %d\n", x);
        }
        else{
                printf("Parent process / pid : %d, childPID : %d\n", (int)getpid(), rc);
                 x = 50;
                printf("x in Parent : %d\n", x);
        }

        return 0;
}
