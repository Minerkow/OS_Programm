#include "Header.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Expected Argument");
        exit(EXIT_FAILURE);
    }
    size_t numChild = Read_Number_from_Text(argv[1]);
    if (argc > 3) {
        fprintf(stderr, "Extra Arguments");
        exit(EXIT_FAILURE);
    }

    struct sigaction childDead;
    childDead.sa_handler = Child_Dead_Handler;
    if (sigfillset(&childDead.sa_mask) < 0) {
        perror("sigfillset");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGCHLD, &childDead, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    struct pollfd *ufdsR = calloc(numChild, sizeof(struct pollfd));
    struct pollfd *ufdsW = calloc(numChild, sizeof(struct pollfd));
    struct Connection_t *connectArr = calloc(numChild, sizeof(struct Connection_t));

#ifdef DEBUG
    fprintf(stderr, "We ready fork\n");
#endif

    for (size_t i = 0; i < numChild; ++i) {
        //Pipe Open C -> P
        int pipeCP[2] = {-1, -1};
        $(pipe(pipeCP))

        connectArr[i].rcvFd = pipeCP[READ];
        ufdsR[i].fd = pipeCP[READ];
        ufdsR[i].events = POLLIN;

        //Pipe Open P -> C
        int pipePC[2] = {-1, -1};
        if (i > 0) {
            $(pipe(pipePC))
            connectArr[i - 1].sendFd = pipePC[WRITE];
            ufdsW[i - 1].fd = pipePC[WRITE];
            ufdsW[i - 1].events = POLLOUT;
        }
        if (i == numChild - 1) {
            connectArr[i].sendFd = STDOUT_FILENO;
            ufdsW[i].fd = STDOUT_FILENO;
            ufdsW[i].events = POLLOUT;
        }
        pid_t ppid = getpid();

        //Create children
        pid_t pid = fork();
        if (pid == 0) {
            $(prctl(PR_SET_PDEATHSIG, SIGKILL))
            if (ppid != getppid()) {
                fprintf(stderr, "Daddy DEAD!\n");
                exit(EXIT_FAILURE);
            }

//            Close signals and free heap
            for (int j = 0; j < i + 1; ++j) {
                assert(connectArr[j].rcvFd != -1);
                $(close(connectArr[j].rcvFd))
                if ((j != numChild - 1) && (i != 0)) {
                    assert(connectArr[j].sendFd != -1);
                    $(close(connectArr[j].sendFd))
                }
            }
            free(ufdsR);
            free(ufdsW);
            free(connectArr);

            if (i == 0) {
                Loader_Run(argv[2], pipeCP[WRITE]);

            } else {
                Child_Run(pipeCP[WRITE], pipePC[READ]);
            }
        } else {
            if (i == 0) {
                assert(pipeCP[WRITE] != -1);
                $(close(pipeCP[WRITE]))
            } else {
                assert(pipePC[READ] != -1);
                $(close(pipeCP[WRITE]))
                $(close(pipePC[READ]))
            }
        }
    }

    for (int i = 0; i < numChild; ++i) {
        //Create buff
        connectArr[i].capacity = Size_Buf(numChild - i);
        connectArr[i].size = 0;
        connectArr[i].buff = calloc(connectArr->capacity, sizeof(char));
        if (connectArr[i].buff == NULL) {
            perror("calloc");
            exit(EXIT_FAILURE);
        }
        connectArr[i].offsetBegin = connectArr[i].buff;
        connectArr[i].offsetEnd = connectArr[i].buff;
        $(fcntl(connectArr[i].rcvFd, F_SETFL, O_NONBLOCK))
        $(fcntl(connectArr[i].sendFd, F_SETFL, O_NONBLOCK))
    }

#ifdef DEBUG
    fprintf(stderr, "ufdsW:[");
    for (int i = 0; i < numChild; ++i) {
        fprintf(stderr, "%d ", ufdsW[i]);
    }
    fprintf(stderr, "]\n");

    fprintf(stderr, "ufdsR:[");
    for (int i = 0; i < numChild; ++i) {
        fprintf(stderr, "%d ", ufdsR[i]);
    }
    fprintf(stderr, "]\n");

    fprintf(stderr, ">>>>>>>>>>>Parrent start work\n");

    Print_All_Buffers(connectArr, numChild);
#endif

    int numCorpses = 0;
    while(1) {

        //Poll write
        int numW = poll(ufdsW, numChild, 0);
#ifdef DEBUG
        fprintf(stderr, "NumW:%d\n", numW);
#endif
        if (numW < 0) {
            perror("pollW");
            exit(EXIT_FAILURE);
        }
        if (numW != 0) {
            for (size_t i = 0; i < numChild; ++i) {
                if (ufdsW[i].revents & POLLOUT) {
#ifdef DEBUG
                    fprintf(stderr, ">>>>>>>>>>>Download to %d\n", ufdsW[i].fd);
#endif
                    Download_From_Buff(&connectArr[i]);
                }

                ufdsW[i].revents = 0;
            }
        }

        //Poll read
        int numR = poll(ufdsR, numChild, 0);
        int numCloseR = 0;
#ifdef DEBUG
        fprintf(stderr, "NumR:%d\n", numR);
#endif
        if (numR < 0) {
            perror("pollR");
            exit(EXIT_FAILURE);
        }
        if (numR != 0) {
            for (size_t i = 0; i < numChild; ++i) {
                if (ufdsR[i].revents & POLLIN) {
#ifdef DEBUG
                    fprintf(stderr, ">>>>>>>>>>>Load from %d\n", ufdsR[i].fd);
#endif
                    Load_To_Buff(&connectArr[i]);
#ifdef DEBUG
                    Print_All_Buffers(connectArr, numChild);
#endif
                } else {
                    if (ufdsR[i].revents & POLLHUP || ufdsR[i].revents & POLLNVAL) {
                        close(connectArr[i].rcvFd);
                        if (connectArr[i].size == 0) {
                            close(connectArr[i].sendFd);
                        }
#ifdef DEBUG
                        Print_All_Buffers(connectArr, numChild);
#endif
                        numCloseR++;
                    }
                }
                ufdsR[i].revents = 0;
            }
        }

        int status = 0;
        int ret = waitpid(-1, &status, WNOHANG);
        if (ret < 0 && errno != ECHILD) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        if (errno != ECHILD && ret != 0) {
            if (WEXITSTATUS(status) != EXIT_SUCCESS) {
                fprintf(stderr, "ERROR, Child DEAD");
                exit(EXIT_FAILURE);
            }
            if (WEXITSTATUS(status) == EXIT_SUCCESS) {
                //fprintf(stderr, "pid  - %d\n", ret);
                numCorpses++;
            }
        }
        //fprintf(stderr, "corpses = %d, numCloser = %d, numR = %d\n", numCorpses, numCloseR, numR);
        if (numCorpses == numChild && numCloseR == numR) {
            Download_From_Buff(&connectArr[numChild - 1]);
            //fprintf(stderr, "END - %d == %d", numCorpses, numChild);
            fflush(stdout);
            return 1;
        }
        fflush(stdout);
    }
}

