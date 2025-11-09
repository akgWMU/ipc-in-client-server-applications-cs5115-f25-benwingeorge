#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

struct message {
    char operation[4];
    int operand1;
    int operand2;
};

int main() {
    const char *fifo_req = "fifo_request";
    const char *fifo_res = "fifo_response";
    mkfifo(fifo_req, 0666);
    mkfifo(fifo_res, 0666);

    printf("Server started. Waiting for client requests...\n");

    int req_fd = open(fifo_req, O_RDONLY);
    int res_fd = open(fifo_res, O_WRONLY);

    struct message msg;
    int result;

    while (1) {
        int bytes = read(req_fd, &msg, sizeof(msg));
        if (bytes <= 0) continue;

        if (strcmp(msg.operation, "exit") == 0) break;

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

        write(res_fd, &result, sizeof(result));
    }

    close(req_fd);
    close(res_fd);
    unlink(fifo_req);
    unlink(fifo_res);
    printf("Server exiting.\n");
    return 0;
}