/* Question 3.
 * mimic a simple web or file server. 
 * Each request should be to read the contents of a file (named in the request)
 * and the server should respond by reading the file into a buffer, 
 * and then returning the contents to the client. 
 *
 * Use the standard open(), read(), close() system calls to implement this feature. 
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345
#define BUF_SIZE 99999

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

    if (listen(listen_fd, 5) < 0) {
        perror("listen"); close(listen_fd); exit(1);
    }

    printf("File server listening on port %d...\n", PORT);

    fd_set master, readfds;
    FD_ZERO(&master);
    FD_SET(listen_fd, &master);
    int max_fd = listen_fd;

    while (1) {
        readfds = master;

        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select"); exit(1);
        }

        for (int fd = 0; fd <= max_fd; fd++) {
            if (FD_ISSET(fd, &readfds)) {
                if (fd == listen_fd) {
                    // New client
                    int client_fd = accept(listen_fd, NULL, NULL);
                    if (client_fd < 0) { perror("accept"); continue; }
                    FD_SET(client_fd, &master);
                    if (client_fd > max_fd) max_fd = client_fd;
                    printf("New client connected (fd=%d)\n", client_fd);
                } else {
                    // Client request (possibly multiple times)
                    char filename[BUF_SIZE];
                    int n = read(fd, filename, sizeof(filename)-1);
                    if (n <= 0) {
                        close(fd);
                        FD_CLR(fd, &master);
                        printf("Client (fd=%d) disconnected.\n", fd);
                    } else {
                        filename[n] = '\0';
                        char *newline = strchr(filename, '\n');
                        if (newline) *newline = '\0';

                        printf("Client (fd=%d) requested file: %s\n", fd, filename);

                        int file_fd = open(filename, O_RDONLY);
                        if (file_fd < 0) {
                            char *err = "Error: cannot open file\n";
                            write(fd, err, strlen(err));
                        } else {
                            char buf[BUF_SIZE];
                            int bytes;
                            while ((bytes = read(file_fd, buf, sizeof(buf))) > 0) {
                                write(fd, buf, bytes);
                            }
                            close(file_fd);
                        }
                    }
                }
            }
        }
    }

    close(listen_fd);
    return 0;
}
