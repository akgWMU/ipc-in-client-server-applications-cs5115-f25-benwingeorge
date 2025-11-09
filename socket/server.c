#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/server_socket"

struct message {
    char operation[8];
    int operand1;
    int operand2;
};

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    struct message msg;
    int result;

    // 1. Create a UNIX domain socket
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Remove any previous socket file and bind
    unlink(SOCKET_PATH);
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Listen for client connections
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server started. Waiting for clients...\n");

    // 4. Accept and handle clients in a loop
    while (1) {
        if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
            perror("accept");
            continue;
        }

        while (1) {
            ssize_t n = read(client_fd, &msg, sizeof(msg));
            if (n <= 0)
                break; // client closed connection

            if (strcmp(msg.operation, "exit") == 0) {
                printf("Client requested exit.\n");
                close(client_fd);
                unlink(SOCKET_PATH);
                close(server_fd);
                printf("Server shutting down.\n");
                exit(EXIT_SUCCESS);
            }

            // Compute the result
            if (strcmp(msg.operation, "add") == 0)
                result = msg.operand1 + msg.operand2;
            else if (strcmp(msg.operation, "sub") == 0)
                result = msg.operand1 - msg.operand2;
            else if (strcmp(msg.operation, "mul") == 0)
                result = msg.operand1 * msg.operand2;
            else if (strcmp(msg.operation, "div") == 0) {
                if (msg.operand2 == 0) {
                    printf("Divide by zero attempt.\n");
                    result = 0;
                } else
                    result = msg.operand1 / msg.operand2;
            } else {
                printf("Invalid operation: %s\n", msg.operation);
                result = 0;
            }

            printf("Server: %s(%d, %d) = %d\n",
                   msg.operation, msg.operand1, msg.operand2, result);

            write(client_fd, &result, sizeof(result));
        }

        close(client_fd);
        printf("Client disconnected.\n");
    }

    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}