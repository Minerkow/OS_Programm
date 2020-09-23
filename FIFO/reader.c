#define _GNU_SOURCE
#include "reader.h"
#include "writer.h"

void Load_from_Fifo_in_Stdin() {
    char* fifo = "/tmp/fifo";
    int fdFifo = open(fifo, O_RDONLY);
    while(1) {
        int numSymbols = splice(fdFifo, NULL, STDOUT_FILENO, NULL, BLOCK_SIZE, SPLICE_F_MOVE);
        if (numSymbols == -1) {
            perror("splice()");
            exit(ERROR);
        }
        if (numSymbols == 0) {
            break;
        }
    }
    int err;
    err = close(fdFifo);
    if (err == -1) {
        perror("close()");
        exit(ERROR);
    }
}