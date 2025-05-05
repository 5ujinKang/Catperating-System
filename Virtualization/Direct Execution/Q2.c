/*
2. Measure Context Switch Overhead
Create two processes that communicate via two UNIX pipes (or sockets), 
using a ping-pong pattern: one process writes to a pipe and waits to read; the other reads and then writes back.

This triggers real context switches between the two processes.

Measure the total round-trip communication time, then divide by the number of context switches to get an average switch cost.
*/
