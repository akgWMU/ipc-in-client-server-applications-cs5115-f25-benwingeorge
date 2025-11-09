#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/server_socket"

struct message
{
    char operation[8];
    int operand1;
    int operand2;
};

int main()
{
    int sock_fd;
    struct sockaddr_un addr;
    struct message msg;
    int result;

    // 1. Create socket
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Connect to server socket
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("connect");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    // 3. Loop for user input
    // while (1)
    // {
    //     printf("Enter operation (add/sub/mul/div or exit): ");
    //     scanf("%7s", msg.operation);

    //     if (
    //         strcmp(msg.operation, "add") != 0 &&
    //         strcmp(msg.operation, "sub") != 0 &&
    //         strcmp(msg.operation, "mul") != 0 &&
    //         strcmp(msg.operation, "div") != 0)
    //     {
    //         printf("Invalid operation: %s\n", msg.operation);
    //         result = 0;
    //     }

    //     // if (strcmp(msg.operation, "exit") == 0 && result != 0)
    //     if (strcmp(msg.operation, "exit") == 0)
    //     {
    //         write(sock_fd, &msg, sizeof(msg));
    //         break;
    //     }

    //     // if (result != 0)
    //     // {
    //         printf("Enter two integers: ");
    //         scanf("%d %d", &msg.operand1, &msg.operand2);

    //         write(sock_fd, &msg, sizeof(msg));
    //         read(sock_fd, &result, sizeof(result));

    //         printf("Result from server: %d\n", result);
    //     // }
    // }

    while (1)
    {
        printf("Enter operation (add/sub/mul/div or exit): ");
        scanf("%7s", msg.operation);

        if (strcmp(msg.operation, "exit") == 0)
        {
            write(sock_fd, &msg, sizeof(msg));
            break;
        }

        if (
            strcmp(msg.operation, "add") != 0 &&
            strcmp(msg.operation, "sub") != 0 &&
            strcmp(msg.operation, "mul") != 0 &&
            strcmp(msg.operation, "div") != 0)
        {
            printf("Invalid operation: %s\n", msg.operation);
            continue; // Go back to the start of the while loop
        }

        printf("Enter two integers: ");
        scanf("%d %d", &msg.operand1, &msg.operand2);

        write(sock_fd, &msg, sizeof(msg));
        read(sock_fd, &result, sizeof(result));

        printf("Result from server: %d\n", result);
    }

    close(sock_fd);
    printf("Client exiting.\n");
    return 0;
}