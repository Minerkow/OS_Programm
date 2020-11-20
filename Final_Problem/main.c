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

    struct pollfd *ufdsR = NULL;
    struct pollfd *ufdsW = NULL;
    struct Connection_t* connectArr = NULL;

//#ifdef DEBUG
    fprintf(stderr, "We ready fork\n");
//#endif

    for (long long int i = 0; i < numChild; ++i) {
        //Pipe Open  C -> P
        int pipeFdCP[2] = {-1, -1};
        pipe(pipeFdCP);

        //Pipe Open P -> C
        int pipeFdPC[2] = {-1, -1};
        if (i > 0) {
            pipe(pipeFdPC);
            connectArr[i - 1].sendFd = pipeFdPC[WRITE];

            //Add fdWrite to WPoll
            assert(ufdsW);
            ufdsW[i-1].fd = pipeFdPC[WRITE];
            ufdsW[i-1].events = POLLOUT;
        }
        if (i == numChild - 1) {
            connectArr[i].sendFd = STDOUT_FILENO;

            //Add fdWrite to WPoll
            assert(ufdsW);
            ufdsW[i].fd = STDOUT_FILENO;
            ufdsW[i].events = POLLOUT;
        }
        pid_t ppid = getpid();

        //Create children
        pid_t pid = fork();
        if (pid == 0) {
            if (prctl(PR_SET_PDEATHSIG, SIGKILL) < 0) {
                perror("prctl()");
                exit(EXIT_FAILURE);
            }

            if (ppid != getppid()) {
                fprintf(stderr, "Daddy DEAD!\n");
                exit(EXIT_FAILURE);
            }

            //Loader Run
            if (i == 0) {
                close(pipeFdCP[READ]);

                Loader_Run(argv[2], pipeFdCP[WRITE]);
                close(pipeFdCP[WRITE]);

                fprintf(stderr, "Loader exit\n");
                exit(EXIT_SUCCESS);
            } else {
                //Close signals and free heap
                close(connectArr[0].rcvFd);
                for (int j = 1; j < i + 1; ++j) {
                    close(connectArr[j].rcvFd);
                    close(connectArr[j].sendFd);
                }
                free(ufdsR);
                free(ufdsW);
                free(connectArr);

                Child_Run(pipeFdCP[WRITE], pipeFdPC[READ]);

                close(pipeFdCP[WRITE]);
                close(pipeFdPC[READ]);

                exit(EXIT_SUCCESS);
            }

        } else {
            //Calloc
            if (i == 0) {
                fprintf(stderr, "Memory alocated\n");

                ufdsR = calloc(numChild, sizeof(struct pollfd));
                ufdsW = calloc(numChild, sizeof(struct pollfd));
                connectArr = calloc(numChild, sizeof(struct Connection_t));

                close(pipeFdCP[WRITE]);
            } else {
                close(pipeFdCP[WRITE]);
                close(pipeFdPC[READ]);
            }

            connectArr[i].rcvFd = pipeFdCP[READ];

            //Add fdRead to RPoll
            assert(ufdsR);
            ufdsR[i].fd = pipeFdCP[READ];
            ufdsR[i].events = POLLIN;
        }
    }

    for (int i = 0; i < numChild; ++i) {
        //Create buff
        connectArr[i].capacity = Size_Buf(numChild - i);
        connectArr[i].size = 0;
        connectArr[i].buff = calloc(connectArr->capacity, sizeof(char));
        connectArr[i].offsetBegin = connectArr[i].buff;
        connectArr[i].offsetEnd = connectArr[i].buff;
    }

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
    int j = 0;
    while(1) {

        //Poll write
        int numW = poll(ufdsW, numChild, 0);
        fprintf(stderr, "NumW:%d\n", numW);
        if (numW < 0) {
            perror("pollW");
            exit(EXIT_FAILURE);
        }
        int k = 0;
        if (numW != 0) {
            for (size_t i = 0; i < numChild; ++i) {
                if (ufdsW[i].revents == POLLOUT) {
                    fprintf(stderr, ">>>>>>>>>>>Download to %d\n", ufdsW[i].fd);
                    Download_From_Buff(&connectArr[i]);
                    k++;
                }
                ufdsW[i].revents = 0;
            }
        }

        //Poll read
        int numR = poll(ufdsR, numChild, 0);
        fprintf(stderr, "NumR:%d\n", numR);
        if (numR < 0) {
            perror("pollR");
            exit(EXIT_FAILURE);
        }
        if (numR != 0) {
            for (size_t i = 0; i < numChild; ++i) {
                if (ufdsR[i].revents == POLLIN) {
                    fprintf(stderr, ">>>>>>>>>>>Load from %d\n", ufdsR[i].fd);
                    Load_To_Buff(&connectArr[i]);
                }
                if ((ufdsR[i].revents == POLLHUP || ufdsR[i].revents == POLLNVAL ||
                    ufdsR[i].revents == POLLERR) && connectArr[i].size == 0) {
                    fprintf(stderr, "kek");
                    if (i != numChild - 1) {
                        close(connectArr[i].sendFd);
                    }
                    close(connectArr[i].rcvFd);
                }
                ufdsR[i].revents = 0;
            }
        }
        Print_All_Buffers(connectArr, numChild);
        j++;
//        if (numR == 0) {
//            Download_From_Buff(&connectArr[numChild - 1]);
//            return 1;
//        }
        if (j == 20) {
            break;
        }
        //exit(2);
    }

    return 2;
}
