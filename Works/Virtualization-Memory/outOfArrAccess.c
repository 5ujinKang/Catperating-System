/*
instruction : valgrind --leak-check=yes ./outOfArrAccess
    ==39317== Memcheck, a memory error detector
    ==39317== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
    ==39317== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
    ==39317== Command: ./outOfArrAccess
    ==39317==
    ==39317== Invalid write of size 4 ****************
    ==39317==    at 0x10918D: main (outOfArrAccess.c:6)
    ==39317==  Address 0x4a8e1d0 is 0 bytes after a block of size 400 alloc'd
    ==39317==    at 0x4848899: malloc (vg_replace_malloc.c:381)
    ==39317==    by 0x10917E: main (outOfArrAccess.c:4)
    ==39317==
    ==39317==
    ==39317== HEAP SUMMARY:
    ==39317==     in use at exit: 0 bytes in 0 blocks
    ==39317==   total heap usage: 1 allocs, 1 frees, 400 bytes allocated
    ==39317==
    ==39317== All heap blocks were freed -- no leaks are possible
    ==39317==
    ==39317== For lists of detected and suppressed errors, rerun with: -s
    ==39317== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
*/


#include <stdlib.h>

int main() {
    int *arr = malloc(100 * sizeof(int));
    
    arr[100] = 0;

    free(arr); 
    return 0;  
}