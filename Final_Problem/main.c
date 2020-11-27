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

    struct Connection_t *connectArr = calloc(numChild, sizeof(struct Connection_t));

#ifdef DEBUG
    fprintf(stderr, "We ready fork\n");
#endif

    for (size_t i = 0; i < numChild; ++i) {
        //Pipe Open C -> P
        int pipeCP[2] = {-1, -1};
        $(pipe(pipeCP))

        connectArr[i].rcvFd = pipeCP[READ];
        connectArr[i].size = 0;

        //Pipe Open P -> C
        int pipePC[2] = {-1, -1};
        if (i > 0) {
            $(pipe(pipePC))
            connectArr[i - 1].sendFd = pipePC[WRITE];

        }
        if (i == numChild - 1) {
            connectArr[i].sendFd = STDOUT_FILENO;

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
    struct Ufds_t ufds = {calloc(2*numChild, sizeof(struct pollfd)),
                          calloc(2*numChild, sizeof(size_t)),
                          0, 0};
    while(1) {
        //Update ufds
//        if (ufds.notCloseIndex == numChild - 1) {
//            return 1;
//        }
        ufds.size = 0;
        for (size_t i = ufds.notCloseIndex; i < numChild; ++i) {
            if (connectArr[i].size == 0) {
                ufds.data[ufds.size].fd = connectArr[i].rcvFd;
                ufds.data[ufds.size].events = POLLIN;
                ufds.buffsInd[ufds.size] = i;
                ufds.size++;
                continue;
            }
            if (connectArr[i].size == connectArr[i].capacity || i == ufds.notCloseIndex) {
                ufds.data[ufds.size].fd = connectArr[i].sendFd;
                ufds.data[ufds.size].events = POLLOUT;
                ufds.buffsInd[ufds.size] = i;
                ufds.size++;
                continue;
            }

            ufds.data[ufds.size].fd = connectArr[i].rcvFd;
            ufds.data[ufds.size].events = POLLIN;
            ufds.buffsInd[ufds.size] = i;
            ufds.size++;

            ufds.data[ufds.size].fd = connectArr[i].sendFd;
            ufds.data[ufds.size].events = POLLOUT;
            ufds.buffsInd[ufds.size] = i;
            ufds.size++;
        }

        if (ufds.size == 0) {
            assert(numCorpses == numChild);
            break;
        }
        //Poll
        int numReadyFd = poll(ufds.data, ufds.size, 0);
        if (numReadyFd < 0) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        if (numReadyFd != 0) {
            for (size_t i = 0; i < ufds.size; ++i) {
                if (ufds.data[i].revents & POLLOUT) {
                    Download_From_Buff(&connectArr[ufds.buffsInd[i]]);
                }
                if (ufds.data[i].revents & POLLIN) {
                    Load_To_Buff(&connectArr[ufds.buffsInd[i]]);
                } else {
                    if (ufds.data[i].revents & POLLHUP || ufds.data[i].revents & POLLNVAL) {
                        close(connectArr[ufds.buffsInd[i]].rcvFd);
                        if (connectArr[i].size == 0) {
                            if (ufds.buffsInd[i] != numChild - 1) {
                                close(connectArr[ufds.buffsInd[i]].sendFd);
                            }
                            ufds.notCloseIndex++;
                        }
                    }
                }
                //Print_All_Buffers(connectArr, numChild);
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
                numCorpses++;
            }
        }
        fflush(stdout);
    }
    for (size_t i = 0; i < numChild; ++i) {
        free(connectArr[i].buff);
    }
    free (connectArr);
    free(ufds.buffsInd);
    free(ufds.data);
}

