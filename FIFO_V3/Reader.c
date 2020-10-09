#include "Reader.h"

void Get_from_Writer() {
    setbuf( stdout, NULL );

    char servFifo[] = "/tmp/fifo.serv";
    char dataFifo[MAX_LEN_FIFO_NAME] = "/tmp/fifo.";
    sprintf(dataFifo + 10, "%d", getpid());

    printf("%s\n", dataFifo);

    if (mkfifo(servFifo, 0666) < 0 && errno != EEXIST) {
        perror("mkfifo()");
        exit(ERROR);
    }

    int fdServ = open(servFifo, O_WRONLY);
    if (fdServ < 0) {
        perror("open()");
        exit(ERROR);
    }

    if (mkfifo(dataFifo, 0666) < 0 && errno != EEXIST) {
        perror("mkfifo()");
        exit(ERROR);
    }

    int fdData = open(dataFifo, O_RDONLY | O_NDELAY);
    if (fdData < 0 && errno != ENOENT) {
        perror("open()");
        exit(ERROR);
    }

    int fdDataWR = open(dataFifo, O_WRONLY | O_NDELAY);
    if (fdData < 0 && errno != ENOENT) {
        perror("open()");
        exit(ERROR);
    }

    printf("open servFifo\n");

    Connect_with_Writer(fdServ);

    printf("connect with writer\n");

    printf("Create DATA FIFO\n");

    printf("open dataFifo\n");


    Get_File(fdData, fdDataWR);
    printf("get File\n");

    if (close(fdServ) < 0) {
        perror("close()");
        exit(ERROR);
    }

    if (close(fdData) < 0) {
        perror("close()");
        exit(ERROR);
    }
}

void Connect_with_Writer(int fdServ) {
    pid_t pidReader = getpid();
    if (write(fdServ, &pidReader, sizeof(pidReader)) < 0) {
        perror("write()");
        exit(ERROR);
    }
}

void Get_File(int fdData, int fdDataWR) {

    fd_set rfds;
    struct timeval tv = {WRITER_TIMEOUT, 0};
    FD_ZERO(&rfds);
    FD_SET(fdData, &rfds);
    int retval;
    retval = select(fdData + 1, &rfds,
                    NULL, NULL, &tv);
    switch (retval) {
        case 0:
            fprintf(stderr, "Writer DEAD!!");
            return;
        case -1:
            perror("select()");
            exit(ERROR);
        default:;
    }

    if (close(fdDataWR) < 0) {
        perror("close()");
        exit(ERROR);
    }
    if (fcntl(fdData, F_SETFL, O_RDONLY)) {
        perror("fcntl()");
        exit(ERROR);
    }

    while (1) {

        int numSymbols = splice( fdData, NULL,
                                 STDOUT_FILENO, NULL,
                                 PIPE_BUF, SPLICE_F_MOVE);
        if (numSymbols < 0) {
            perror("splice()");
            exit(ERROR);
        }
        if (numSymbols == 0)
            break;
    }
}
