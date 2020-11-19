#include "Source.h"

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

    Child_Dead_Handler(-numChild);

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
        fprintf(stderr, "P#%d:C -> P, %d -> %d\n", i, pipeFdCP[WRITE], pipeFdCP[READ]);

        //Pipe Open P -> C
        int pipeFdPC[2] = {-1, -1};
        if (i != 0) {
            pipe(pipeFdPC);
            fprintf(stderr, "P#%d:P -> C, %d -> %d\n", i, pipeFdPC[WRITE], pipeFdPC[READ]);
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
            fprintf(stderr, "C#%d:C -> P, %d -> %d\n", i, pipeFdCP[WRITE], pipeFdCP[READ]);

            fprintf(stderr, "C#%d:P -> C, %d -> %d\n", i, pipeFdPC[WRITE], pipeFdPC[READ]);

            if (ppid != getppid()) {
                fprintf(stderr, "Daddy DEAD!\n");
                exit(EXIT_FAILURE);
            }

            //close(pipeFdCP[READ]);

            //Loader Run
            if (i == 0) {
                Loader_Run(argv[2], pipeFdCP[WRITE]);
                fprintf(stderr, "Loader exit\n");
                exit(EXIT_SUCCESS);
            } else {
                //Child Run

                Child_Run(pipeFdCP[WRITE], pipeFdPC[READ]);

                //fprintf(stderr, "I am child, #%d\n", i);

                //close(pipeFdPC[WRITE]);
                //fprintf(stderr, "Close %d pid - %d \n", pipeFdPC[WRITE], getpid());
                //close(pipeFdCP[READ]);
                //fprintf(stderr, "Close %d pid - %d \n", pipeFdPC[WRITE], getpid());

                exit(EXIT_SUCCESS);
            }

        } else {
            //Calloc
            if (i == 0) {

                fprintf(stderr, "Memory alocated\n");

                ufdsR = calloc(numChild, sizeof(struct pollfd));
                ufdsW = calloc(numChild, sizeof(struct pollfd));
                connectArr = calloc(numChild, sizeof(struct Connection_t));
            }

            connectArr[i].rcvFd = pipeFdCP[READ];

            //Add fdRead to RPoll
            assert(ufdsR);
            ufdsR[i].fd = pipeFdCP[READ];
            ufdsR[i].events = POLLIN;

            //Create buff
            connectArr[i].capacity = Size_Buf(numChild - i);
            connectArr[i].size = 0;
            connectArr[i].buff = calloc(connectArr->capacity, sizeof(char));
            connectArr[i].offsetBegin = connectArr[i].buff;
            connectArr[i].offsetEnd = connectArr[i].buff;

            fprintf(stderr, "Buff #%d create\n", i);

            //close(pipeFdCP[WRITE]);
            //close(pipeFdPC[READ]);
        }
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
        if (numW != 0) {
            for (size_t i = 0; i < numChild; ++i) {
                if (ufdsW[i].revents == POLLOUT) {
                    fprintf(stderr, ">>>>>>>>>>>Download to %d\n", ufdsW[i].fd);
                    Download_From_Buff(&connectArr[i]);
                }
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
            }
        }
        Print_All_Buffers(connectArr, numChild);
        j++;
        if (j == 200) break;
        //exit(2);
    }

    return 0;
}
