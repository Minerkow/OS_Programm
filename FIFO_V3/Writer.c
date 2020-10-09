#include "Writer.h"

void Send_to_Reader(char path[]) {
    char dataFifo[MAX_LEN_FIFO_NAME] = "/tmp/fifo.";
    char servFifo[] = "/tmp/fifo.serv";
    pid_t pidReader;

    int fdFile = open(path, O_RDONLY);
    if (fdFile < 0) {
        perror("open()");
        exit(ERROR);
    }

    printf("open File\n");

    if (mkfifo(servFifo, 0666) < 0 && errno != EEXIST) {
        perror("mkfifo()");
        exit(ERROR);
    }

    int fdServ = open(servFifo, O_RDONLY);
    if (fdServ < 0) {
        perror("open()");
        exit(ERROR);
    }

    printf("open servFifo\n");

    Connect_with_Reader(dataFifo, fdServ, &pidReader);

    //sleep(5);

    printf("%s\n", dataFifo);

    printf("connect with reader\n");

    if (mkfifo(dataFifo, 0666) < 0 && errno != EEXIST) {
        perror("mkfifo()");
        exit(ERROR);
    }

    int fdData = open(dataFifo, O_WRONLY | O_NDELAY);
    if (fdData < 0) {
        perror("open()");
        exit(ERROR);
    }

    printf("open dataFifo\n");

    Send_File( fdFile, fdData, pidReader);

    printf("send file\n");

    if (close(fdFile) < 0) {
        perror("close()");
        exit(ERROR);
    }

    if (close(fdServ) < 0) {
        perror("close()");
        exit(ERROR);
    }

    if (close(fdData) < 0) {
        perror("close()");
        exit(ERROR);
    }

    if (unlink(dataFifo) < 0) {
        perror("unlink()");
        exit(ERROR);
    }8
}

void Connect_with_Reader(char dataFifo[], int fdServ, pid_t* pidReader) {
    if (read(fdServ, pidReader, sizeof(pid_t)) < 0) {
        perror("write()");
        exit(ERROR);
    }
    if (*pidReader == 0) {
        fprintf(stderr, "Reader busy");
        exit(ERROR);
    }
    sprintf(dataFifo + 10, "%d", *pidReader);
}

void Send_File(int fdFile, int fdData, pid_t pidReader) {
    if (fcntl(fdData, F_SETFL, O_WRONLY)) {
        perror("fcntl()");
        exit(ERROR);
    }

    while (1) {
        int numSymbols = splice(fdFile, NULL, fdData, NULL, PIPE_BUF, SPLICE_F_MOVE);
        if (numSymbols < 0) {
            perror("splice");
            exit(ERROR);
        }
        if (numSymbols == 0)
            break;
    }
}