/*
1. Measure System Call Overhead
Write a small program that repeatedly invokes a simple system call, such as a 0-byte read().

Use gettimeofday() to time the total duration of the repeated calls.

Divide the total time by the number of iterations to get the average cost of a system call.

To assess timer accuracy, measure back-to-back calls to gettimeofday(). If it’s not precise enough, consider using rdtsc.

*/
