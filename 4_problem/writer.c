#include "writer.h"

void Writer(const char* path, pid_t rpid) {
    int fdFile = open(path, O_RDONLY);
    if (fdFile < 0) {
        perror("open()");
        exit(EXIT_FAILURE);
    }

    struct sigaction confirm;
    confirm.sa_handler = Confirm_Handler;
    if (sigfillset(&confirm.sa_mask) < 0) {
        perror("sigfillset()");
        exit(EXIT_FAILURE);
    }
    if (sigaction(CONFIRM, &confirm, NULL) < 0) {
        perror("sigaction()");
        exit(EXIT_FAILURE);
    }


    //Send data

    fprintf(stderr, "begin send\n");

    sigset_t expectSig;

    if (sigaddset(&expectSig, CONFIRM) < 0) {
        perror("sigaddset()");
        exit(EXIT_FAILURE);
    }
    if (sigprocmask(SIG_BLOCK, &expectSig, NULL) < 0) {
        perror("sigprocmask()");
        exit(EXIT_FAILURE);
    }

    if (sigemptyset(&expectSig) < 0) {
        perror("sigemptyset()");
        exit(EXIT_FAILURE);
    }

    char sym;
    sigset_t sigSet;
    if (sigemptyset(&sigSet) < 0) {
        perror("sigemptyset()");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int err = read(fdFile, &sym, 1);
        if (err < 0) {
            perror("read()");
            exit(EXIT_FAILURE);
        }
        if (err == 0) {
            break;
        }
        for (int i = 128; i >= 1; i /= 2) {
            if (i & sym) {
                fprintf(stderr, "send1\n");
                kill(rpid, ONE_SIGNAL);
            } else {
                fprintf(stderr, "send0\n");
                kill(rpid, ZERO_SIGNAL);
            }


            if (sigsuspend(&sigSet) < 0) {
                perror("sigsuspend(w)");
                exit(EXIT_FAILURE);
            }
        }
    }
    exit(EXIT_SUCCESS);
}

void Confirm_Handler(int sig) {
    fprintf(stderr, "confirm\n");
}