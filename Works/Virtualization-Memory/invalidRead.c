/*
    instruction : valgrind --leak-check=yes ./invalidRead
    ==42857== Memcheck, a memory error detector
    ==42857== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
    ==42857== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
    ==42857== Command: ./invalidRead
    ==42857==
    ==42857== Invalid read of size 4
    ==42857==    at 0x1091B7: main (invalidRead.c:10)
    ==42857==  Address 0x4a8e04c is 12 bytes inside a block of size 400 free'd
    ==42857==    at 0x484B27F: free (vg_replace_malloc.c:872)
    ==42857==    by 0x1091AE: main (invalidRead.c:8)
    ==42857==  Block was alloc'd at
    ==42857==    at 0x4848899: malloc (vg_replace_malloc.c:381)
    ==42857==    by 0x10919E: main (invalidRead.c:6)
    ==42857==
    0==42857==
    ==42857== HEAP SUMMARY:
    ==42857==     in use at exit: 0 bytes in 0 blocks
    ==42857==   total heap usage: 2 allocs, 2 frees, 1,424 bytes allocated
    ==42857==
    ==42857== All heap blocks were freed -- no leaks are possible
    ==42857==
    ==42857== For lists of detected and suppressed errors, rerun with: -s
    ==42857== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
*/


#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr = malloc(100 * sizeof(int));
    
    free(arr); 

    printf("%d", arr[3]);
    return 0;  
}