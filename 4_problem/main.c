#include <stdio.h>
#include "reader.h"
#include "writer.h"

int main(int argc, char** argv) {
    if (argc > 2) {
        fprintf(stderr, "Extra Arguments");
        exit(EXIT_FAILURE);
    }
    if (argc < 2) {
        fprintf(stderr, "Expected arguments");
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    switch (pid) {
        case 0:
            fprintf(stderr, "I am child");
            Writer(argv[1], getppid());
            break;
        case -1:
            perror("fork()");
            exit(EXIT_FAILURE);
        default:
            fprintf(stderr, "I am parrent!\n");
            Reader(pid);
    }
    return 0;
}
