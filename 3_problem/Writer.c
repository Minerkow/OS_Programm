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

    int semId = semget(key, 6, 0666 | IPC_CREAT );
    if (semId < 0) {
        perror("semgetW");
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    fprintf(stderr, "ReaderSem out!\n");
        unsigned short vals[6];
        if (semctl(semId, 0, GETALL, vals) < 0) {
            perror("semctl()");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "SemVal = {");
        for (int i = 0; i < 6; ++i) {
            fprintf(stderr, " %d", vals[i]);
        }
        fprintf(stderr, "}\n");
#endif

#ifdef DELETE
    DeleteSem(semId);
#endif

    struct sembuf semInstructions[3];

    semInstructions[0].sem_num = WRITER;
    semInstructions[0].sem_op = W;
    semInstructions[0].sem_flg = 0;

    semInstructions[1].sem_num = WRITER;
    semInstructions[1].sem_op = V;
    semInstructions[1].sem_flg = SEM_UNDO;

    //Init MUTEX
    if (semctl(semId, MUTEX, SETVAL, 1) < 0) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }


    //Init EMPTY
    if (semctl(semId, EMPTY, SETVAL, 1) < 0) {
        perror("semctl()");
        exit(EXIT_FAILURE);
    }

    //Init CONNECT
    semInstructions[2].sem_num = CONNECT;
    semInstructions[2].sem_op = V;
    semInstructions[2].sem_flg = SEM_UNDO;

    if (semop(semId, semInstructions, 3) < 0) {
        perror("semop");
        exit(EXIT_FAILURE);
    }

    //Writer ready
    struct sembuf writerReady = {WRITER, V, SEM_UNDO};
    if (semop(semId, &writerReady, 1) < 0) {
        perror("semop()");
        exit(EXIT_FAILURE);
    }

    //Wait Connect
    struct sembuf checkC[2];

    checkC[0].sem_num = READER;
    checkC[0].sem_op = P2;
    checkC[0].sem_flg = 0;

    checkC[1].sem_num = READER;
    checkC[1].sem_op = V2;
    checkC[1].sem_flg = 0;

    semop(semId, checkC, 2);

    //Recv data

    struct Data_t buff;

    while (1) {
        buff.numSymbols_ = read(fdFile, buff.data_, DATASIZE);
        if (buff.numSymbols_ < 0) {
            perror("read()");
            exit(EXIT_FAILURE);
        }

        //EMPTY - P && MUTEX - P
#ifdef DEBUG
        fprintf(stderr, "ReaderSem out!\n");
        if (semctl(semId, 0, GETALL, vals) < 0) {
            perror("semctl()");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "SemVal = {");
        for (int i = 0; i < 6; ++i) {
            fprintf(stderr, " %d", vals[i]);
        }
        fprintf(stderr, "}\n");
#endif
        struct sembuf sopEP[3];

        sopEP[0].sem_num = READER;
        sopEP[0].sem_op = P;
        sopEP[0].sem_flg = IPC_NOWAIT;

        sopEP[1].sem_num = READER;
        sopEP[1].sem_op = V;
        sopEP[1].sem_flg = 0;

        sopEP[2].sem_num = EMPTY;
        sopEP[2].sem_op = P;
        sopEP[2].sem_flg = 0;

        if (semop(semId, sopEP, 3) < 0) {
            perror ("semop writer in while");
            fprintf(stderr, "Reader DEAD\n");
            exit(EXIT_FAILURE);
        }

        struct sembuf sopMP = {MUTEX, P, SEM_UNDO};
        semop(semId, &sopMP, 1);

#ifdef DEBUG
    fprintf(stderr, "MUTEX and EMPTY - P\n");
#endif

        if (memcpy(shMemory, &buff, SHMSIZE) == NULL) {
            fprintf(stderr, "MemCpy ERROR\n");
            exit(EXIT_FAILURE);
        }

        //FULL - V && MUTEX - V
        struct sembuf sopFV = {FULL, V, 0};
        semop(semId, &sopFV, 1);
        struct sembuf sopMV = {MUTEX, V, SEM_UNDO};
        semop(semId, &sopMV, 1);

#ifdef DEBUG
        fprintf(stderr, "MUTEX and FULL - V\n");
#endif

#ifdef DEBUG
        fprintf(stderr, "ReaderSem out!\n");
        if (semctl(semId, 0, GETALL, vals) < 0) {
            perror("semctl()");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "SemVal = {");
        for (int i = 0; i < 6; ++i) {
            fprintf(stderr, " %d", vals[i]);
        }
        fprintf(stderr, "}\n");
#endif

        if (buff.numSymbols_ == 0) {
            break;
        }
    }

    struct sembuf pairP = {CONNECT, P, SEM_UNDO};
    semop(semId, &pairP, 1);

    struct sembuf pairW = {CONNECT, W, 0};
    semop(semId, &pairW, 1);

    struct sembuf sop = {WRITER, P, SEM_UNDO};
    if (semop(semId, &sop, 1) < 0) {
        perror("semop4");
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    fprintf(stderr, "ReaderSem out!\n");
        if (semctl(semId, 0, GETALL, vals) < 0) {
            perror("semctl()");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "SemVal = {");
        for (int i = 0; i < 6; ++i) {
            fprintf(stderr, " %d", vals[i]);
        }
        fprintf(stderr, "}\n");
#endif

    if (close(fdFile) < 0) {
        perror("close()");
        exit(EXIT_FAILURE);
    }

}


//----------------------------------------------------------------------------------------

void DeleteSem(int semId) {
    if (semctl(semId, 0, IPC_RMID) < 0) {
        perror("semctl()");
        exit(EXIT_FAILURE);
    }
}
