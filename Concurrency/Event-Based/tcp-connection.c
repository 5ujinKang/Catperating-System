/*
 * Question 1. 
 * write a simple server that can accept and serve TCP connections. 
 * Build this to serve exactly one request at a time
 * have each request be very simple, e.g., to get the current time of day
 * */
// simple_time_server.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 12345
#define BACKLOG 1

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) { perror("socket"); exit(1); }

    struct sockaddr_in serv = {0};
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);

    if (bind(listen_fd, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
        perror("bind"); close(listen_fd); exit(1);
    }

    if (listen(listen_fd, BACKLOG) < 0) {
        perror("listen"); close(listen_fd); exit(1);
    }

    printf("Listening on port %d...\n", PORT);

    while (1) {
        int client_fd = accept(listen_fd, NULL, NULL);
        if (client_fd < 0) { perror("accept"); continue; }

        // Generate current time string
        time_t now = time(NULL);
        char buf[128];
        snprintf(buf, sizeof(buf), "Current time: %s", ctime(&now));

        write(client_fd, buf, strlen(buf));
        close(client_fd);
        printf("Served one client, connection closed.\n");
    }

    close(listen_fd);
    return 0;
}

