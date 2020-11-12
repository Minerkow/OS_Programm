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

    int semId = semget(key, 5, 0666 | IPC_CREAT );
    if (semId < 0) {
        perror("semgetW");
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    fprintf(stderr, "ReaderSem out!\n");
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

#ifdef DELETE
    DeleteSem(semId);
#endif

    struct sembuf semInstructions[2];

    semInstructions[0].sem_num = WRITER;
    semInstructions[0].sem_op = W;
    semInstructions[0].sem_flg = 0;

    semInstructions[1].sem_num = WRITER;
    semInstructions[1].sem_op = V;
    semInstructions[1].sem_flg = SEM_UNDO;

    //Init MUTEX 
    semInstructions[2].sem_num = MUTEX;
    semInstructions[2].sem_op = V;
    semInstructions[2].sem_flg = SEM_UNDO;

    if (semop(semId, semInstructions, 2) < 0) {
        perror("semop");
        exit(EXIT_FAILURE);
    }

    //Init FULL

    struct sembuf checkF = {FULL, W, IPC_NOWAIT};
    if (semop(semId, &checkF, 1) < 0) {
        struct sembuf initF = {FULL, P, 0};
        semop(semId, &initF, 1);
    }

    //Init EMPTY

    struct sembuf checkE = {EMPTY, W, IPC_NOWAIT};
    if (semop(semId, &checkE, 1) >= 0) {
        struct sembuf initE = {EMPTY, V, 0};
        semop(semId, &initE, 1);
    }

    //Recv data

    struct Data_t buff;

    while (1) {
        buff.numSymbols_ = read(fdFile, buff.data_, DATASIZE);
        if (buff.numSymbols_ < 0) {
            perror("read()");
            exit(EXIT_FAILURE);
        }

        //EMPTY - P && MUTEX - P
        struct sembuf sopEP = {EMPTY, P, 0};
        semop(semId, &sopEP, 1);
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
        for (int i = 0; i < 5; ++i) {
            fprintf(stderr, " %d", vals[i]);
        }
        fprintf(stderr, "}\n");
#endif

        if (buff.numSymbols_ == 0) {
            break;
        }
    }



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


//----------------------------------------------------------------------------------------

void DeleteSem(int semId) {
    if (semctl(semId, 0, IPC_RMID) < 0) {
        perror("semctl()");
        exit(EXIT_FAILURE);
    }
}
