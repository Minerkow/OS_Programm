#include "reader.h"

static char rcvSym = 0;
static int numBit = 128;
static pid_t writerPid;

void Reader(pid_t wpid) {
    writerPid = wpid;
    struct sigaction rcvOne;
    rcvOne.sa_handler = One_Handler;
    if (sigfillset(&rcvOne.sa_mask) < 0) {
        perror("sigfillset()");
        exit(EXIT_FAILURE);
    }
    if (sigaction(ONE_SIGNAL, &rcvOne, NULL) < 0) {
        perror("sigaction()");
        exit(EXIT_FAILURE);
    }

    struct sigaction rcvZero;
    rcvZero.sa_handler = Zero_Handler;
    if (sigfillset(&rcvOne.sa_mask) < 0) {
        perror("sigfillset()");
        exit(EXIT_FAILURE);
    }
    if (sigaction(ZERO_SIGNAL, &rcvZero, NULL) < 0) {
        perror("sigaction()");
        exit(EXIT_FAILURE);
    }

    struct sigaction childDead;
    childDead.sa_handler = Dead_Child_Handler;
    if (sigfillset(&childDead.sa_mask) < 0) {
        perror("sigfillset()");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGCHLD, &childDead, NULL) < 0) {
        perror("sigaction()");
        exit(EXIT_FAILURE);
    }


    //Blocks:

    sigset_t sigSet;

    if (sigaddset(&sigSet, SIGCHLD) < 0) {
        perror("sigaddset()");
        exit(EXIT_FAILURE);
    }
    if (sigaddset(&sigSet, ZERO_SIGNAL) < 0) {
        perror("sigaddset()");
        exit(EXIT_FAILURE);
    }
    if (sigaddset(&sigSet, ONE_SIGNAL) < 0) {
        perror("sigaddset()");
        exit(EXIT_FAILURE);
    }
    if (sigprocmask(SIG_BLOCK, &sigSet, NULL) < 0) {
        perror("sigprocmask()");
        exit(EXIT_FAILURE);
    }

    //Clear expect sig set

    if (sigemptyset(&sigSet) < 0) {
        perror("sigfillset()");
        exit(EXIT_FAILURE);
    }

    //Receive data

    fprintf(stderr, "Begin receive!\n");

    while (1) {
        if (numBit == 0) {
            printf("%c", rcvSym);
            fflush(0);
            numBit = 128;
            rcvSym = 0;
        }

        if (sigsuspend(&sigSet) < 0) {
            perror("sigsuspend(r)");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}

void One_Handler(int sig) {
    fprintf(stderr, "1\n");
    rcvSym += numBit;
    numBit /= 2;
    if (kill(writerPid, CONFIRM) < 0) {
        perror("kill()");
        exit(EXIT_FAILURE);
    }
}

void Zero_Handler(int sig) {
    fprintf(stderr, "0\n");
    numBit /= 2;
    if (kill(writerPid, CONFIRM) < 0) {
        perror("kill()");
        exit(EXIT_FAILURE);
    }
}

void Dead_Child_Handler(int sig) {
    fprintf(stderr, "Child DEAD!");
    exit(EXIT_SUCCESS);
}