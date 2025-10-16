/*
    usage of valgrind & gdb

    1. compile : gcc -g -o null null.c

    2-1. gdb : gdb null
        result :
        GNU gdb (Ubuntu 12.1-0ubuntu1~22.04.2) 12.1
        Copyright (C) 2022 Free Software Foundation, Inc.
        License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
        This is free software: you are free to change and redistribute it.
        There is NO WARRANTY, to the extent permitted by law.
        Type "show copying" and "show warranty" for details.
        This GDB was configured as "x86_64-linux-gnu".
        Type "show configuration" for configuration details.
        For bug reporting instructions, please see:
        <https://www.gnu.org/software/gdb/bugs/>.
        Find the GDB manual and other documentation resources online at:
            <http://www.gnu.org/software/gdb/documentation/>.

        For help, type "help".
        Type "apropos word" to search for commands related to "word"...
        Reading symbols from null...
        (gdb) run
        Starting program: /mnt/c/Users/water/ostep-Sujin-2/Virtualization-Memory/null
        [Thread debugging using libthread_db enabled]
        Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

        Program received signal SIGSEGV, Segmentation fault.
        0x0000555555555161 in main () at null.c:5
        5           printf("%d\n", *ptr);
    
    
    2-2. valgrind --leak-check=yes ./null
    result : 
        ==34091== Memcheck, a memory error detector
        ==34091== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
        ==34091== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
        ==34091== Command: ./null
        ==34091==
        ==34091== Invalid read of size 4
        ==34091==    at 0x109161: main (null.c:5)
        ==34091==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
        ==34091==
        ==34091==
        ==34091== Process terminating with default action of signal 11 (SIGSEGV)
        ==34091==  Access not within mapped region at address 0x0
        ==34091==    at 0x109161: main (null.c:5)
        ==34091==  If you believe this happened as a result of a stack
        ==34091==  overflow in your program's main thread (unlikely but
        ==34091==  possible), you can try to increase the size of the
        ==34091==  main thread stack using the --main-stacksize= flag.
        ==34091==  The main thread stack size used in this run was 8388608.
        ==34091==
        ==34091== HEAP SUMMARY:
        ==34091==     in use at exit: 0 bytes in 0 blocks
        ==34091==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
        ==34091==
        ==34091== All heap blocks were freed -- no leaks are possible
        ==34091==
        ==34091== For lists of detected and suppressed errors, rerun with: -s
        ==34091== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
        Segmentation fault (core dumped)
*/


#include <stdio.h>

int main() {
    int *ptr = NULL; 
    printf("%d\n", *ptr); 
    return 0;
}