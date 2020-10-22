#include "msg.h"


long long int Read_Number_from_Bash (int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Expected Argument");
        exit(EXIT_FAILURE);
    }
    if (argc > 2) {
        fprintf(stderr, "Extra Arguments");
        exit(EXIT_FAILURE);
    }
    char **endptr;
    long long int num = strtoll(argv[1], endptr, 10);
    if (num < 0) {
        fprintf(stderr, "Less 0");
        exit(EXIT_FAILURE);
    }
    if (num == LONG_MAX) {
        fprintf(stderr, "Big number");
        exit(EXIT_FAILURE);
    }
    if (**endptr != '\0') {
        fprintf(stderr, "Wrong format");
        exit(EXIT_FAILURE);
    }
    return num;
}

void Create_Children(long long int num) {

    int mqid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    if (mqid < 0) {
        perror("msgget()");
        exit(EXIT_FAILURE);
    }
    Free_Msg(mqid);

    int ppid = getpid();
    struct msgBuff_t* msgs = calloc(num, sizeof(struct msgBuff_t));

    for (long long int i = 0; i < num; ++i) {
        pid_t pid = fork();
        switch (pid) {
            case -1: {
                perror("fork()");
                Free_Msg(0);
                exit(EXIT_FAILURE);
            }
            case 0: {
                if (ppid != getppid()) {
                    fprintf(stderr, "Not my Dad!!");
                    Free_Msg(0);
                    exit(EXIT_FAILURE);
                }
                struct msgBuff_t msg;
                if (msgrcv(mqid, &msg, MSG_SIZE, getpid(), 0) < 0) {
                    perror("msgrcv()");
                    Free_Msg(0);
                    exit(EXIT_FAILURE);
                }
                printf("I am child with pid - %d, my id - №%lld\n", getpid(), msg.id_);
                fflush(0);
                struct msgBuff_t msgForSend = {getppid(), ACCEPT};
                if (msgsnd(mqid, &msgForSend, MSG_SIZE, 0) < 0) {
                    perror("msgsend()");
                    Free_Msg(0);
                    exit(EXIT_FAILURE);
                }
                exit(EXIT_SUCCESS);
            }
            default: {
                msgs[i].id_ = i;
                msgs[i].mtype_ = pid;
                break;
            }
        }
    }

    fprintf(stderr, "I am dady with pid - %d\n", getpid());

    for (long long int i = 0; i < num; ++i) {
        if (msgsnd(mqid, &msgs[i], MSG_SIZE,0) < 0) {
            perror("msgsnd()");
            Free_Msg(0);
            exit(EXIT_FAILURE);
        }
        struct msgBuff_t msg;
        if (msgrcv(mqid, &msg, MSG_SIZE, getpid(), 0) < 0) {
            perror("msgrcv()");
            Free_Msg(0);
            exit(EXIT_FAILURE);
        }
    }

    free(msgs);
    Free_Msg(0);
}

void Free_Msg(int id) {
    static int mqid = 0;
    if (id != 0) {
        mqid = id;
    } else {
        if (msgctl(mqid, IPC_RMID, NULL) < 0 && errno != EINVAL) {
            perror("msgctl()");
            exit(EXIT_FAILURE);
        }
    }
}