/* Question 4. 
 * Now, instead of using standard I/O system calls, 
 * use the asynchronous I/O interfaces as described in the chapter. 
 * How hard was it to incorporate asynchronous interfaces into your program?
 * : The code is longer and complicated.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <aio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12346
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

    printf("Async File Server listening on port %d...\n", PORT);

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
                    int client_fd = accept(listen_fd, NULL, NULL);
                    if (client_fd < 0) { perror("accept"); continue; }
                    FD_SET(client_fd, &master);
                    if (client_fd > max_fd) max_fd = client_fd;
                    printf("New client connected (fd=%d)\n", client_fd);
                } else {
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

                        printf("Client requested file: %s\n", filename);

                        int file_fd = open(filename, O_RDONLY);
                        if (file_fd < 0) {
                            char *err = "Error: cannot open file\n";
                            write(fd, err, strlen(err));
                        } else {
                            // Set up async I/O control block
                            struct aiocb aio;
                            memset(&aio, 0, sizeof(aio));
                            char *buf = malloc(BUF_SIZE);
                            aio.aio_fildes = file_fd;
                            aio.aio_buf = buf;
                            aio.aio_nbytes = BUF_SIZE;
                            aio.aio_offset = 0;

                            // Start async read
                            if (aio_read(&aio) < 0) {
                                perror("aio_read");
                                free(buf);
                                close(file_fd);
                                continue;
                            }

                            // Wait for completion
                            while (aio_error(&aio) == EINPROGRESS) {
                                // busy wait for demo (could use aio_suspend)
                            }

                            int ret = aio_return(&aio);
                            if (ret > 0) {
                                write(fd, buf, ret);
                            }

                            free(buf);
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

