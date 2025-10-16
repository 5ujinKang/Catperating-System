/*

Q3. create a little program that uses a certain amount of memory,
 called memory-user.c. This program should take one command
line argument: the number of megabytes of memory it will use.
 Whenrun,itshouldallocateanarray,andconstantlystreamthrough
 the array, touching each entry. The program should do this indefi
nitely, or, perhaps, for a certain amount of time also specified at the
 commandline.

 */

/*

Q4. Now, while running your memory-user program, also (in a dif
ferent terminal window, but on the same machine) run the free
 tool. How do the memory usage totals change when your program
 is running? How about whenyoukill the memory-userprogram?
 Do the numbers match your expectations? Try this for different
 amounts of memory usage. What happens when you use really
 large amounts of memory? : used memory increased / available & free memory decreased

- none / result of free : 
                 total        used        free      shared  buff/cache   available
    Mem:         3936500      463968     3277296        3860      195236     3312348
    Swap:        1048576           0     1048576


 - When run with 50MB (./memuser 50) 
 result of free :
                 total        used        free      shared  buff/cache   available
    Mem:         3936500      515104     3226292        3860      195104     3261216
    Swap:        1048576           0     1048576


 - When run with 500MB (./memuser 500)
 result of free : 
                total        used        free      shared  buff/cache   available
    Mem:         3936500     1396756     2344688        3860      195056     2379524
    Swap:        1048576           0     1048576
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <MB>\n", argv[0]);
        return 1;
    }
    int mb = atoi(argv[1]);
    size_t size = mb * 1024 * 1024;
    char *array = malloc(size);
    if (array == NULL) {
        fprintf(stderr, "Allocation failed\n");
        return 1;
    }
    printf("Allocated %d MB\n", mb);

    while (1) {
        for (size_t i = 0; i < size; i += 4096) {
            array[i] = 0; // touch each page
        }
        sleep(1);
    }
    free(array);
    return 0;
}