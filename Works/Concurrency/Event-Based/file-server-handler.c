/* Question 5.
 * add some signal handling to your code. 
 * One common use of signals is to poke a server to reload some kind of configuration file, 
 * or take some other kind of administrative action. 
 *
 * Perhaps one natural way to play around with this is to add a user-level file cache to your server, 
 * which stores recently accessed files. 
 *
 * Implement a signal handler that clears the cache when the signal is sent to the server process.
 *
 * : the speed is just same. 
 *   guess that's because code is really simple and the way to find in cache is for loop not like hash table
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345
#define BUF_SIZE 1024
#define CACHE_SIZE 5

typedef struct {
    char filename[128];
    char content[BUF_SIZE];
    int valid;
} cache_entry;

cache_entry file_cache[CACHE_SIZE];

// Signal handler: clear cache
void handle_sigusr1(int signum) {
    printf("Signal received: clearing cache\n");
    for (int i = 0; i < CACHE_SIZE; i++) {
        file_cache[i].valid = 0;
    }
}

int main() {
    signal(SIGUSR1, handle_sigusr1);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) { perror("socket"); exit(1); }

    struct sockaddr_in serv = {0};
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);

    if (bind(listen_fd, (struct sockaddr*)&serv, sizeof(serv)) < 0) { perror("bind"); exit(1); }
    if (listen(listen_fd, 5) < 0) { perror("listen"); exit(1); }

    printf("Server with cache listening on port %d...\n", PORT);

    fd_set master, readfds;
    FD_ZERO(&master);
    FD_SET(listen_fd, &master);
    int max_fd = listen_fd;

    while (1) {
        readfds = master;
        if (select(max_fd+1, &readfds, NULL, NULL, NULL) < 0) { 
		if (errno == EINTR) continue;
		perror("select"); exit(1); 
	}

        for (int fd = 0; fd <= max_fd; fd++) {
            if (!FD_ISSET(fd, &readfds)) continue;

            if (fd == listen_fd) {
                int client_fd = accept(listen_fd, NULL, NULL);
                if (client_fd >= 0) {
                    FD_SET(client_fd, &master);
                    if (client_fd > max_fd) max_fd = client_fd;
                }
            } else {
                char filename[128];
                int n = read(fd, filename, sizeof(filename)-1);
                if (n <= 0) {
                    close(fd);
                    FD_CLR(fd, &master);
                    continue;
                }
                filename[n] = '\0';
                char *newline = strchr(filename, '\n');
                if (newline) *newline = '\0';

                // Check cache
                int found = 0;
                for (int i = 0; i < CACHE_SIZE; i++) {
                    if (file_cache[i].valid && strcmp(file_cache[i].filename, filename) == 0) {
                        write(fd, file_cache[i].content, strlen(file_cache[i].content));
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    // Read from file
                    int file_fd = open(filename, O_RDONLY);
                    if (file_fd < 0) {
                        char *err = "Error: cannot open file\n";
                        write(fd, err, strlen(err));
                    } else {
                        char buf[BUF_SIZE];
                        int bytes = read(file_fd, buf, sizeof(buf)-1);
                        if (bytes > 0) {
                            buf[bytes] = '\0';
                            write(fd, buf, bytes);

                            // Store in cache (overwrite first empty slot)
                            for (int i = 0; i < CACHE_SIZE; i++) {
                                if (!file_cache[i].valid) {
                                    strncpy(file_cache[i].filename, filename, sizeof(file_cache[i].filename)-1);
                                    strncpy(file_cache[i].content, buf, sizeof(file_cache[i].content)-1);
                                    file_cache[i].valid = 1;
                                    break;
                                }
                            }
                        }
                        close(file_fd);
                    }
                }
            }
        }
    }

    close(listen_fd);
    return 0;
}

