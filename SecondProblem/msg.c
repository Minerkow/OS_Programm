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

    struct msgBuff_t* msgs = calloc(num, sizeof(struct msgBuff_t));

    for (long long int i = 0; i < num; ++i) {
        pid_t pid = fork();
        switch (pid) {
            case -1: {
                perror("fork()");
                exit(EXIT_FAILURE);
            }
            case 0: {
                if (prctl(PR_SET_PDEATHSIG, mqid) < 0) {
                    perror("prctl()");
                    exit(EXIT_FAILURE);
                }
                if (signal(mqid, Handler) < 0) {
                    perror("signal()");
                    exit(EXIT_FAILURE);
                }
                struct msgBuff_t msg;
//                if (i == 20) {
//                    sleep(10);
//                }
                if (msgrcv(mqid, &msg, sizeof(struct msgBuff_t) - sizeof(long),
                        getpid(), 0) < 0) {
                    perror("msgrcv()");
                    exit(EXIT_FAILURE);
                }
                printf("I am process with pid - %d, my №%lld\n", getpid(), msg.id_);
                exit(EXIT_SUCCESS);
            }
            default: {
                msgs[i].id_ = i;
                msgs[i].mtype_ = pid;
                break;
            }
        }
    }

    fprintf(stderr, "%d ", getpid());

    for (long long int i = 0; i < num; ++i) {
        if (msgsnd(mqid, &msgs[i], sizeof(struct msgBuff_t) - sizeof(long),
                0) < 0) {
            perror("msgsnd()");
            exit(EXIT_FAILURE);
        }
        int stat;
        if (waitpid(msgs[i].mtype_, &stat, 0) < 0) {
            perror("waitpid()");
            exit(EXIT_FAILURE);
        }
        if (WIFSIGNALED(stat) != 0 && WTERMSIG(stat) != EXIT_SUCCESS) {
            fprintf(stderr, "Process with pid - %ld, DEAD\n", msgs[i].mtype_);
        }
    }

    if (msgctl(mqid, IPC_RMID, NULL) < 0) {
        perror("msgctl()");
        exit(EXIT_FAILURE);
    }
}

void Handler(int signum) {
    int err = msgctl(signum, IPC_RMID, NULL);
    if (err < 0 && errno != EINVAL) {
        perror("msgctl()");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_FAILURE);
}