#define _GNU_SOURCE
#include "reader.h"

void Load_from_Fifo_in_Stdin() {
    setbuf( stdout, NULL );
    int err;
    char* dataFifo = "/tmp/dataFifo";
    char* ansFifo = "/tmp/ansFifo";
    pid_t pidWriter;

    int fdData;
    fdData = open(dataFifo, O_RDONLY);
    PerrorCheck(fdData == -1, "open()");

    err = mkfifo(ansFifo, 0666);
    //PerrorCheck(err == -1, "mkfifo()");

    printf("Open data, Create FIFO\n");

    int fdAns = open(ansFifo, O_WRONLY);
    PerrorCheck(fdAns == -1, "open()");

    printf("open ansFIFO\n");

    Connect_with_Writer(fdAns, fdData, &pidWriter);

    while(Receive_Message(fdData, pidWriter)) {}

    err = close(fdData);
    PerrorCheck(err == -1, "close()");

    err = close(fdAns);
    PerrorCheck(err == -1, "close()");

    err = unlink(ansFifo);
    PerrorCheck(err == -1, "unlink()");
}

void Connect_with_Writer(int fdAns, int fdData, pid_t* pidWriter) {
    int err;
    struct ConnectAccept_t cac;
    struct Message_t mess;
    cac.pidReader = getpid();

    printf("Hello\n");

    while(true) {
        err = read(fdData, &mess, sizeof(mess));
        PerrorCheck(err == -1, "read()");
        if (mess.pidReader == CONNECT)
            break;
    }

    *pidWriter = mess.pidWriter;
    cac.pidWriter = mess.pidWriter;

    err = write(fdAns, &cac, sizeof(cac));
    PerrorCheck(err == -1, "write()");

    printf("\n[Hello, my pid - %d, I connected with writer - %d]\n", getpid(), *pidWriter);
}

bool Receive_Message(int fdData, pid_t pidWriter) {
    int err;
    struct Message_t mess;

    while (true) {
        err = read(fdData, &mess, sizeof(mess));
        PerrorCheck(err == -1, "read()");
        if (mess.pidWriter != pidWriter)
            kill(mess.pidWriter, SIGKILL);
        if (mess.pidReader == getpid() && mess.pidWriter == pidWriter)
            break;
    }
    printf("\n[Hello, my pid - %d, I receive message - %d, size - %d]\n", getpid(), pidWriter, mess.size);

    for (unsigned i = 0; i < mess.size; ++i) {
        printf("%c", mess.data[i]);
    }
    if (mess.size < DATA_SIZE)
        return false;
    return true;
}