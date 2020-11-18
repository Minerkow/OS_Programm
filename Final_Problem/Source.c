#include "Source.h"

long long int Read_Number_from_Text(const char* text) {
    char **endptr;
    long long int num = strtoll(text, endptr, 10);
    if (num < 0) {
        fprintf(stderr, "Less 0");
        exit(EXIT_FAILURE);
    }
    if (num == LONG_MAX) {
        fprintf(stderr, "Big number");
        exit(EXIT_FAILURE);
    }
    if (**endptr != '\0') {
        fprintf(stderr, "Wrong format");
        exit(EXIT_FAILURE);
    }
    return num;
}

void Child_Run(int sendFd, int rcvFd) {
    while (1) {
        int numSymbols = splice( rcvFd, NULL,
                                 sendFd, NULL,
                                 PIPE_BUF, SPLICE_F_MOVE);
        if (numSymbols < 0) {
            perror("splice()");
            exit(EXIT_FAILURE);
        }
        if (numSymbols == 0)
            break;
    }
}

void Loader_Run(char* path, int sendFd) {
    int fileFd = open(path, O_RDONLY);
    while (1) {

        int numSymbols = splice( fileFd, NULL,
                                 sendFd, NULL,
                                 PIPE_BUF, SPLICE_F_MOVE);
        if (numSymbols < 0) {
            perror("splice()");
            exit(EXIT_FAILURE);
        }
        if (numSymbols == 0)
            break;
    }
}