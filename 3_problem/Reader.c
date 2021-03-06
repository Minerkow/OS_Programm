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

    int semId = semget(key, 6, 0666 | IPC_CREAT );
    if (semId < 0) {
        perror("semget");
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

    struct sembuf semInstructions[4];

    semInstructions[0].sem_num = READER;
    semInstructions[0].sem_op = W;
    semInstructions[0].sem_flg = 0;

    semInstructions[1].sem_num = READER;
    semInstructions[1].sem_op = V;
    semInstructions[1].sem_flg = SEM_UNDO;

/*(2)Begin Reader-Reader ShMemory [58-215]*/

    if (semop(semId, semInstructions, 2) < 0) {
        perror("semop1");
        exit(EXIT_FAILURE);
    }

/*(6*)Begin Reader-Writer for FULL [65-71]*/
    //Init FULL
    if (semctl(semId, FULL, SETVAL, 0) < 0) {
        perror("semctl()");
        exit(EXIT_FAILURE);
    }
/*(6*)End Reader-Writer for FULL*/


    //Init CONNECT
    semInstructions[2].sem_num = CONNECT;
    semInstructions[2].sem_op = V;
    semInstructions[2].sem_flg = SEM_UNDO;

    if (semop(semId, &semInstructions[2], 1) < 0) {
        perror("semop1");
        exit(EXIT_FAILURE);
    }

    //Reader ready
    struct sembuf readerReady = {READER, V, SEM_UNDO};
    if (semop(semId, &readerReady, 1) < 0) {
        perror("semop()");
        exit(EXIT_FAILURE);
    }

    //Wait Connect
    struct sembuf checkC[2];

    checkC[0].sem_num = WRITER;
    checkC[0].sem_op = P2;
    checkC[0].sem_flg = 0;

    checkC[1].sem_num = WRITER;
    checkC[1].sem_op = V2;
    checkC[1].sem_flg = 0;

    semop(semId, checkC, 2);

/*(5*)Begin Writer-Reader ShMemory [104-187]*/
    struct Data_t buff;
    while(1) {
        //FULL - P && MUTEX - P
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
        struct sembuf sopFP[3];

        sopFP[0].sem_num = WRITER;
        sopFP[0].sem_op = P;
        sopFP[0].sem_flg = IPC_NOWAIT;

        sopFP[1].sem_num = WRITER;
        sopFP[1].sem_op = V;
        sopFP[1].sem_flg = 0;

        sopFP[2].sem_num = FULL;
        sopFP[2].sem_op = P;
        sopFP[2].sem_flg = 0;

        if (semop(semId, sopFP, 3) < 0) {
            fprintf(stderr, "Writer DEAD\n");
            exit(EXIT_FAILURE);
        }
/*(3*)Begin Reader-Writer for MUTEX [138-141]*/
        struct sembuf sopMP = {MUTEX, P, SEM_UNDO};
        semop(semId, &sopMP, 1);
/*(3*)End Reader-Writer for MUTEX*/


#ifdef DEBUG
        fprintf(stderr, "MUTEX and FULL - P\n");
#endif

        if (memcpy(&buff, shMemory, SHMSIZE) == NULL) {
            fprintf(stderr, "MemCpy ERROR\n");
            exit(EXIT_FAILURE);
        }

/*(4*)Begin Reader-Writer for EMPTY [153 - 157]*/
        //EMPTY - V && MUTEX - V
        struct sembuf sopEV = {EMPTY, V, 0};
        semop(semId, &sopEV, 1);
/*(4*)End Reader-Writer for EMPTY*/

        struct sembuf sopMV = {MUTEX, V, SEM_UNDO};
        semop(semId, &sopMV, 1);
#ifdef DEBUG
        fprintf(stderr, "MUTEX and EMPTY - V\n");
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
        if (write(STDOUT_FILENO, buff.data_, buff.numSymbols_) < 0) {
            perror("write()");
            exit(EXIT_FAILURE);
        }
        //sleep(3);
    }

/*(5*)End Writer-Reader ShMemory*/


    struct sembuf pairP = {CONNECT, P, SEM_UNDO};
    semop(semId, &pairP, 1);

    struct sembuf pairW = {CONNECT, W, 0};
    semop(semId, &pairW, 1);

    struct sembuf sop = {READER, P, SEM_UNDO};
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

/*(2)End Reader-Reader ShMemory*/

}