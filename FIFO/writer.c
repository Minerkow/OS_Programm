#define _GNU_SOURCE
#include "writer.h"

//void Load_into_Pipe(FILE* file) {
//
//
//}

void Load_from_File_in_Fifo(char way[]) {
    int fdFile = open(way, O_RDONLY);
    char* fifo = "/tmp/fifo";
    mkfifo(fifo, 0666);
    int fdFifo = open(fifo, O_WRONLY);
    while (1) {
        int numSymbols = splice(fdFile, NULL, fdFifo, NULL, BLOCK_SIZE, SPLICE_F_MOVE);
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
    err = close(fdFile);
    if (err == -1) {
        perror("close()");
        exit(ERROR);
    }
}