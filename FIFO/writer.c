#define _GNU_SOURCE
#include "writer.h"
#include "ErrorCheck.h"


void Load_from_File_in_Fifo(char way[]) {
    setbuf( stdout, NULL );
    printf ("Hello\n");
    int err;
    char* dataFifo = "/tmp/dataFifo";
    char* ansFifo = "/tmp/ansFifo";
    pid_t pidReader;

    int fdFile = open(way, O_RDONLY);
    PerrorCheck(fdFile == -1, "open()");

    printf("File open\n");

    err = mkfifo(dataFifo, 0666);
    //PerrorCheck(err == -1, "mkfifo()");

    printf("Create Fifo");

    int fdData = open(dataFifo, O_WRONLY);
    PerrorCheck(fdData == -1, "open()");

    printf("open dFifo\n");

    int fdAns = open(ansFifo, O_RDONLY);
    PerrorCheck(fdAns == -1, "open()");

    printf("open ansFifo\n");

    Connect_with_Reader(fdData, fdAns, &pidReader);

    while (Send_Message(fdFile, fdData, pidReader)) {}

    err = close(fdData);
    PerrorCheck(err == -1, "close()");

    err = close(fdFile);
    PerrorCheck(err == -1, "close()");

    err = close(fdAns);
    PerrorCheck(err == -1, "close()");

    err = unlink(dataFifo);
    PerrorCheck(err == -1, "unlink()");
}

void Connect_with_Reader(int fdData, int fdAns, pid_t *pidReader) {
    int err;
    struct Message_t mess = { CONNECT, getpid(),
                            0, {0}};

    err = write(fdData, &mess, sizeof(mess));
    PerrorCheck(err == -1, "write()");

    printf("Send coonect invite, my pid - %d\n", getpid());

    struct ConnectAccept_t cac;

    while(true) {
        err = read(fdAns, &cac, sizeof(cac));
        PerrorCheck(err == -1, "read()");
        if (cac.pidWriter == getpid()) {
            *pidReader = cac.pidReader;
            break;
        }
    }
    printf("\n[Hello, my pid - %d, I connected with reader - %d", getpid(), cac.pidReader);
}

bool Send_Message(int fdFile, int fdData, pid_t pidReader) {
    int err;
    struct Message_t mess;
    mess.pidReader = pidReader;
    mess.pidWriter = getpid();

    int numSymbols = read(fdFile, mess.data, DATA_SIZE);
    PerrorCheck(numSymbols == -1, "read()");

    mess.pidReader = pidReader;
    mess.pidWriter = getpid();
    mess.size = numSymbols;

    err = write(fdData, &mess, sizeof(mess));
    PerrorCheck(err == -1, "write()");

    printf("\n[Hello, my pid - %d, I send message to pid - %d, size - %d]\n", getpid(), pidReader, numSymbols);

    if (numSymbols == 0)
        return false;
    return true;
}