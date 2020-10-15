#include "Writer.h"


void Writer(char* path) {

    int fdFile = open(path, O_RDONLY);
    if (fdFile < 0) {
        perror("open()");
        exit(EXIT_FAILURE);
    }

    const char* PATH = "/tmp";

    key_t key = ftok(PATH, PROJ_ID);
    if (key < 0) {
        perror("ftok()");
        exit(EXIT_FAILURE);
    }


    int idShm = shmget(key, SHMSIZE, 0666 | IPC_CREAT);
    if (idShm < 0) {
        perror("shmget()");
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    fprintf(stderr, "id ShMemory - %d\n", idShm);
#endif


    struct Data_t* shMemory = (struct Data_t*)shmat(idShm, NULL, 0);
    if ((void*)shMemory < 0) {
        perror("shmat()");
        exit(EXIT_FAILURE);
    }

    unsigned short valueSem[5] =  {1, 1, 0, 1, 1};
    int sem = InitSem(key, valueSem, 5);

#ifdef DEBUG
    fprintf(stderr, "All create, sems Init\n");
#endif

    SemOp(sem, P, WRITER, 0);

#ifdef DEBUG
    fprintf(stderr, "Writer LOCK\n");
#endif

    struct Data_t buff;

    while (1) {
        buff.numSymbols_ = read(fdFile, buff.data_, DATASIZE);
        if (buff.numSymbols_ < 0) {
            perror("read()");
            exit(EXIT_FAILURE);
        }

        SemOp(sem, P, EMPTY, 0);
        SemOp(sem, P, MUTEX, 0);

#ifdef DEBUG
    fprintf(stderr, "Start send block Memory!\n");
#endif

        if (memcpy(shMemory, &buff, SHMSIZE) == NULL) {
            fprintf(stderr, "MemCpy ERROR\n");
            exit(EXIT_FAILURE);
        }

#ifdef DEBUG
        fprintf(stderr, "End send block Memory!\n");
#endif

        SemOp(sem, V, MUTEX, 0);
        SemOp(sem, V, FULL, 0);

        if (buff.numSymbols_ == 0) {
            break;
        }
    }

    SemOp(sem, V, WRITER, 0);

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

    if (close(fdFile) < 0) {
        perror("close()");
        exit(EXIT_FAILURE);
    }
}


void SemOp(int sem, short sem_op, int sem_num, short flag) {
    struct sembuf sop = {sem_num, sem_op, flag};
    if (semop(sem, &sop, 1) < 0) {
        perror("semop()");
        exit(EXIT_FAILURE);
    }
}

int InitSem(key_t key, unsigned short* value, int numSem) {
    int semId = semget(key, numSem, 0666 | IPC_CREAT | IPC_EXCL);
    if (semId >= 0) {
#ifdef DEBUG
        fprintf(stderr, "SemId = %d\n", semId);
#endif
        if (semctl(semId, 0, SETALL, value) < 0){
            perror("semctl()");
            exit(EXIT_FAILURE);
        }
#ifdef DEBUG
        unsigned short vals[5];
        if (semctl(semId, 0, GETALL, vals) < 0) {
            perror("semctl()");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "SemVal = {");
        for (int i = 0; i < 5; ++i) {
            fprintf(stderr, " %d", vals[i]);
        }
        fprintf(stderr, "}\n");
#endif

    } else {
        if (errno == EEXIST) {
            semId = semget(key, 0, 0666);
#ifdef DELETE
            DeleteSem(semId);
            exit(EXIT_FAILURE);
#endif
            if (semId < 0) {
                perror("semget()");
                exit(EXIT_FAILURE);
            }
#ifdef DEBUG
            fprintf(stderr, "SemId = %d\n", semId);
#endif

            struct semid_ds stat;
            stat.sem_otime = 0;
            while (stat.sem_otime == 0) {
                if (semctl(semId, 0, IPC_STAT, &stat) < 0) {
                    perror("semctl()");
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            perror("semget()");
            exit(EXIT_FAILURE);
        }
    }
    return semId;
}

void DeleteSem(int semId) {
    if (semctl(semId, 0, IPC_RMID) < 0) {
        perror("semctl()");
        exit(EXIT_FAILURE);
    }
}
