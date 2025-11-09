#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

struct message {
    char operation[4];
    int operand1;
    int operand2;
};

int main() {
    const char *fifo_req = "fifo_request";
    const char *fifo_res = "fifo_response";

    int req_fd = open(fifo_req, O_WRONLY);
    int res_fd = open(fifo_res, O_RDONLY);

    struct message msg;
    int result;

    while (1) {
        printf("Enter operation (add/sub/mul/div or exit): ");
        scanf("%s", msg.operation);

        if (strcmp(msg.operation, "exit") == 0) {
            write(req_fd, &msg, sizeof(msg));
            break;
        }

        printf("Enter two integers: ");
        scanf("%d %d", &msg.operand1, &msg.operand2);

        write(req_fd, &msg, sizeof(msg));
        read(res_fd, &result, sizeof(result));

        printf("Result from server: %d\n", result);
    }

    close(req_fd);
    close(res_fd);
    return 0;
}
