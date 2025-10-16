/*
    instruction : valgrind --leak-check=yes ./leak
        ==36900== Memcheck, a memory error detector
        ==36900== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
        ==36900== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
        ==36900== Command: ./leak
        ==36900==
        ==36900==
        ==36900== HEAP SUMMARY:
        ==36900==     in use at exit: 400 bytes in 1 blocks
        ==36900==   total heap usage: 1 allocs, 0 frees, 400 bytes allocated
        ==36900==
        ==36900== 400 bytes in 1 blocks are definitely lost in loss record 1 of 1 *******
        ==36900==    at 0x4848899: malloc (vg_replace_malloc.c:381)
        ==36900==    by 0x10915E: main (leak.c:4)
        ==36900==
        ==36900== LEAK SUMMARY:
        ==36900==    definitely lost: 400 bytes in 1 blocks *******
        ==36900==    indirectly lost: 0 bytes in 0 blocks
        ==36900==      possibly lost: 0 bytes in 0 blocks
        ==36900==    still reachable: 0 bytes in 0 blocks
        ==36900==         suppressed: 0 bytes in 0 blocks
        ==36900==
        ==36900== For lists of detected and suppressed errors, rerun with: -s
        ==36900== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
*/


#include <stdlib.h>

int main() {
    int *arr = malloc(100 * sizeof(int));
    // free(arr); 
    return 0;  
}