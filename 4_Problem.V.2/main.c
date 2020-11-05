#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/prctl.h>


int rcvSym = 0;
int numBit = 128;
pid_t pid;

void Child_Dead_Handler(int signo);
void Confirm_Handler(int signo);
void One_Handler(int signo);
void Zero_Handler(int signo);
void Daddy_Dead_Handler(int signo);


int main(int argc, char ** argv){
    if (argc > 2) {
        fprintf(stderr, "Many arguments");
        exit(EXIT_FAILURE);
    }
    if (argc < 2) {
        fprintf(stderr, "Few arguments");
        exit(EXIT_FAILURE);
    }
    pid_t ppid = getpid();

    sigset_t set;

    if (sigaddset(&set, SIGUSR1) < 0) {
        perror("sigaddset");
        exit(EXIT_FAILURE);
    }
    if (sigaddset(&set, SIGUSR2) < 0) {
        perror("sigaddset");
        exit(EXIT_FAILURE);
    }
    if (sigaddset(&set, SIGCHLD) < 0) {
        perror("sigaddset");
        exit(EXIT_FAILURE);
    }
    if (sigaddset(&set, SIGALRM) < 0) {
        perror("sigaddset");
        exit(EXIT_FAILURE);
    }
    if (sigprocmask(SIG_BLOCK, &set, NULL) < 0) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

//    if (sigemptyset(&set) < 0) {
//        perror("sigemptyset");
//        exit(EXIT_FAILURE);
//    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {

        if (prctl(PR_SET_PDEATHSIG, SIGKILL) < 0) {
            perror("prctl()");
            exit(EXIT_FAILURE);
        }

        if (ppid != getppid()) {
            fprintf(stderr, "Daddy DEAD!\n");
            exit(EXIT_FAILURE);
        }

        if (sigfillset(&set) < 0) {
            perror("sigemptyset");
            exit(EXIT_FAILURE);
        }
        sigdelset(&set, SIGUSR1);

        struct sigaction confirm;
        confirm.sa_handler = Confirm_Handler;
        if (sigfillset(&confirm.sa_mask) < 0) {
            perror("sigfillset");
            exit(EXIT_FAILURE);
        }
        if (sigaction(SIGUSR1, &confirm, NULL) < 0) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
        
        int fdFile = open(argv[1], O_RDONLY);
        if (fdFile < 0 ){
            perror("open()");
            exit(EXIT_FAILURE);
        }

        char sym = 0;
        while (1){
            int numSym = read(fdFile, &sym, 1);
            if (numSym < 0) {
                perror("read()");
                exit(EXIT_FAILURE);
            }
            if (numSym == 0) {
                break;
            }
            for (int i = 128; i >= 1; i /= 2){
                if (i & sym ) {
                    if (kill(ppid, SIGUSR1) < 0) {
                        perror("kill");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    if (kill(ppid, SIGUSR2) < 0) {
                        perror("kill");
                        exit(EXIT_FAILURE);
                    }
                }
                
                sigsuspend(&set);
            }
        }
        fprintf(stderr, "I want DEI\n");
        exit(EXIT_SUCCESS);
    }

    struct sigaction childDead;
    childDead.sa_handler = Child_Dead_Handler;
    childDead.sa_flags = SA_NOCLDWAIT;

    if (sigfillset(&childDead.sa_mask) < 0) {
        perror("sigfillset");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGCHLD, &childDead, NULL)) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    struct sigaction rcvOne;
    rcvOne.sa_handler = One_Handler;
    if (sigfillset(&rcvOne.sa_mask) < 0) {
        perror("sigfillset");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR1, &rcvOne, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    struct sigaction rcvZero;
    rcvZero.sa_handler = Zero_Handler;
    if (sigfillset(&rcvZero.sa_mask) < 0) {
        perror("sigfillset");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR2, &rcvZero, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sigfillset(&set);
    sigdelset(&set, SIGUSR1);
    sigdelset(&set, SIGUSR2);
    sigdelset(&set, SIGCHLD);

    while(1) {
        if(numBit == 0){
            printf("%c", rcvSym);
            fflush(0);
            numBit=128;
            rcvSym = 0;
        }
        
        sigsuspend(&set);
        if (kill(pid, SIGUSR1) < 0) {
            perror("kill");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}


void Child_Dead_Handler(int signo) {
    fprintf(stderr, "Child dead!\n");
    exit(EXIT_SUCCESS);
}

void Confirm_Handler(int signo) {
    fprintf(stderr, "Confirm\n");
}

void One_Handler(int signo) {
    rcvSym += numBit;
    numBit /= 2;
    fprintf(stderr, "rcv1\n");
}

void Zero_Handler(int signo) {
    numBit/=2;
    fprintf(stderr, "rcv0\n");
}

//void Daddy_Dead_Handler(int signo) {
//    fprintf(stderr, "Daddy DEAD!");
//    exit(EXIT_FAILURE);
//}
