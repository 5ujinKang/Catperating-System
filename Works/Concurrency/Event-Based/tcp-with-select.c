/*
 * Question 2.
 * Simple TCP server with select()
 * - Accept multiple clients
 * - Reply with current time
 * - Minimal implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 12346

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) { perror("socket"); exit(1); }

    struct sockaddr_in serv = {0};
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);

    if (bind(listen_fd, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
        perror("bind"); exit(1);
    }
    if (listen(listen_fd, 5) < 0) {
        perror("listen"); exit(1);
    }

    fd_set master, read_set;
    FD_ZERO(&master);
    FD_SET(listen_fd, &master);
    int max_fd = listen_fd;

    printf("Listening on port %d...\n", PORT);

    while (1) {
        read_set = master;
        if (select(max_fd+1, &read_set, NULL, NULL, NULL) < 0) {
            perror("select"); exit(1);
        }

        for (int fd = 0; fd <= max_fd; fd++) {
            if (!FD_ISSET(fd, &read_set)) continue;

            if (fd == listen_fd) {
                // New connection
                int client_fd = accept(listen_fd, NULL, NULL);
                if (client_fd >= 0) {
                    FD_SET(client_fd, &master);
                    if (client_fd > max_fd) max_fd = client_fd;
                }
            } else {
                // Client sent data (or closed)
                char tmp[1];
                int n = read(fd, tmp, sizeof(tmp));
                if (n <= 0) {
                    // closed
                    close(fd);
                    FD_CLR(fd, &master);
                } else {
                    // Send time
                    time_t now = time(NULL);
                    char buf[128];
                    snprintf(buf, sizeof(buf), "Current time: %s", ctime(&now));
                    write(fd, buf, strlen(buf));
                }
            }
        }
    }
}

