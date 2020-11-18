#include "Source.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Expected Argument");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "%s", argv[1]);
    long long int numChild = Read_Number_from_Text(argv[1]);
    if (argc > 3) {
        fprintf(stderr, "Extra Arguments");
        exit(EXIT_FAILURE);
    }

    struct pollfd *ufdsR = calloc(numChild, sizeof(struct pollfd));
    struct pollfd *ufdsW = calloc(numChild - 1, sizeof(struct pollfd));

    struct Connection_t* connectArr = calloc(numChild, sizeof(struct Connection_t));
    for (long long int i = 0; i < numChild; ++i) {
        //Pipe Open  C -> P
        int pipeFdCP[2];
        pipe(pipeFdCP);
        connectArr[i].rcvFd = pipeFdCP[0];

        //Add fdRead to RPoll
        ufdsR->fd = connectArr[i].rcvFd;
        ufdsR->events = POLLIN;

        //Pipe Open P -> C
        int pipeFdPC[2];
        if (i != 0) {
            pipe(pipeFdPC);
            connectArr[i - 1].sendFd = pipeFdPC[1];

            //Add fdWrite to WPoll
            ufdsW->fd = connectArr[i - 1].sendFd;
            ufdsW->events = POLLOUT;
        }

        //Create children
        pid_t pid = fork();
        if (pid == 0) {

            close(pipeFdCP[READ]);

            //Loader Run
            if (i == 0) {
                Loader_Run(argv[2], pipeFdCP[WRITE]);
                continue;
            }

            close(pipeFdPC[WRITE]);

            //Child Run
            Child_Run(pipeFdCP[WRITE], pipeFdPC[READ]);

            close(pipeFdPC[WRITE]);
            close(pipeFdCP[READ]);

            exit(EXIT_SUCCESS);
        } else {
            //Create buff
            connectArr[i].capacity = Size_Buf(numChild - i);
            connectArr[i].size = 0;
            connectArr[i].buff = calloc(connectArr->size, sizeof(char));
            connectArr[i].offsetBegin = connectArr[i].buff;
            connectArr[i].offsetEnd = connectArr[i].buff;

            close(pipeFdCP[WRITE]);
            close(pipeFdPC[READ]);
        }
    }

    while(1) {

        //Poll write
        int numW = poll(ufdsW, numChild, 0);
        if (numW < 0) {
            perror("pollW");
            exit(EXIT_FAILURE);
        }
        if (numW != 0) {
            for (size_t i = 0; i < numChild - 1; ++i) {
                if (ufdsW[i].revents == POLLOUT) {
                        Download_From_Buff(&connectArr[i]);
                    }
                }
            }

        //Poll read
        int numR = poll(ufdsR, numChild, 0);
        if (numR < 0) {
            perror("pollR");
            exit(EXIT_FAILURE);
        }
        if (numR != 0) {
            for (size_t i = 0; i < numChild; ++i) {
                if (ufdsR[i].revents == POLLIN)
                    Load_To_Buff(&connectArr[i]);
            }
        }
    }

    return 0;
}
