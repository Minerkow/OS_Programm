#include "Reader.h"

void Reader() {
    const char* PATH = "/tmp";

    key_t key = ftok(PATH, PROJ_ID);
    if (key < 0) {
        perror("ftok()");
        exit(EXIT_FAILURE);
    }


    int idShm = shmget(key, SHMSIZE, 0666 | IPC_CREAT );
    if (idShm < 0) {
        perror("shmget()");
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    fprintf(stderr, "id ShMemory - %d\n", idShm);
#endif

    char* shMemory = (char*)shmat(idShm, NULL, 0);
    if ((void*)shMemory < 0) {
        perror("shmat()");
        exit(EXIT_FAILURE);
    }

    unsigned short valueSem[5] =  {1, 1, 0, 1, 1};
    int sem = InitSem(key, valueSem, 5);


#ifdef DEBUG
    fprintf(stderr, "All create, sems Init\n");
#endif

    SemOp(sem, P, READER, SEM_UNDO);

    struct Data_t buff;

    while(1) {
        SemOp(sem, P, FULL, 0);
        SemOp(sem, P, MUTEX, SEM_UNDO);

#ifdef DEBUG
        fprintf(stderr, "Start get block Memory!\n");
#endif

        if (memcpy(&buff, shMemory, SHMSIZE) == NULL) {
            fprintf(stderr, "MemCpy ERROR\n");
            exit(EXIT_FAILURE);
        }

        SemOp(sem, V, MUTEX, SEM_UNDO);
        SemOp(sem, V, EMPTY, 0);

        if (buff.numSymbols_ == 0) {
            break;
        }
        if (write(STDOUT_FILENO, buff.data_, buff.numSymbols_) < 0) {
            perror("write()");
            exit(EXIT_FAILURE);
        }
    }

    SemOp(sem, V, READER, SEM_UNDO);

#ifdef DEBUG
    fprintf(stderr, "ReaderSem out!\n");
        unsigned short vals[5];
        if (semctl(sem, 0, GETALL, vals) < 0) {
            perror("semctl()");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "SemVal = {");
        for (int i = 0; i < 5; ++i) {
            fprintf(stderr, " %d", vals[i]);
        }
        fprintf(stderr, "}\n");
#endif
}