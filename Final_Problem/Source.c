#include "Source.h"

extern size_t numChild;

long long int Read_Number_from_Text(const char* text) {
    char **endptr = calloc(1, sizeof(char*));
    long long int num = strtoll(text, endptr, 10);
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

void Child_Run(int sendFd, int rcvFd) {
    fprintf(stderr, "I download from %d to %d\n", rcvFd, sendFd);
    while (1) {
        int numSymbols = splice( rcvFd, NULL,
                                 sendFd, NULL,
                                 PIPE_BUF, SPLICE_F_MOVE);
        if (numSymbols < 0) {
            perror("splice()");
            fprintf(stderr, "ERROR in fd - %d, %d", rcvFd, sendFd);
            exit(EXIT_FAILURE);
        }
        if (numSymbols == 0)
            break;
    }
}

void Loader_Run(char* path, int sendFd) {
    int fileFd = open(path, O_RDONLY);
    fprintf(stderr, "I download from fileFd: %d to %d\n", fileFd, sendFd);
    while (1) {

        int numSymbols = splice( fileFd, NULL,
                                 sendFd, NULL,
                                 PIPE_BUF, SPLICE_F_MOVE);
        fprintf(stderr, "Loader send %d bytes\n", numSymbols);
        if (numSymbols < 0) {
            perror("splice()");
            fprintf(stderr, "ERROR in fd - fileFD - %d, %d", fileFd, sendFd);
            exit(EXIT_FAILURE);
        }
        if (numSymbols == 0)
            break;
    }
}

size_t Size_Buf(size_t degree) {
    int size = 1;
    for (size_t i = 0; i < degree; ++i) {
        size *= 3;
    }
    return size * 2;
}

void Load_To_Buff(struct Connection_t* buff) {
    if (buff->size == buff->capacity) {
        return;
    }
    if (buff->size == 0) {
        buff->offsetBegin = buff->buff;
        buff->offsetEnd = buff->buff;
        int numSym = read(buff->rcvFd, buff->buff, buff->capacity);
        if (numSym < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        buff->size += numSym;
        buff->offsetEnd += numSym;
        return;
    }

    while (buff->size != buff->capacity) {
        if (buff->offsetEnd == buff->buff + buff->capacity) {
            buff->offsetEnd = buff->buff;
        }
        if (buff->offsetBegin == buff->buff + buff->capacity) {
            buff->offsetBegin = buff->buff;
        }
        if (buff->offsetEnd - buff->offsetBegin > 0) {
            size_t lenToEnd = buff->buff + buff->capacity - buff->offsetEnd;
            int numSym = read(buff->rcvFd, buff->offsetBegin, lenToEnd);
            if (numSym < 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            buff->size += numSym;
            buff->offsetEnd += numSym;
            if (numSym < lenToEnd) {
                break;
            }
            continue;
        } else {
            size_t freeData = buff->offsetBegin - buff->offsetEnd;
            assert(freeData == buff->capacity - buff->size);
            int numSym = read(buff->rcvFd, buff->offsetEnd, freeData);
            if (numSym < 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            buff->size += numSym;
            buff->offsetEnd += numSym;
            if (numSym < freeData) {
                break;
            }
            continue;
        }
        assert(1 && "ooooops");
    }
}

void Download_From_Buff(struct Connection_t* buff) {
    if (buff->size == 0) {
        buff->offsetBegin = buff->buff;
        buff->offsetEnd = buff->buff;
        return;
    }
    while (buff->size != 0) {
        if (buff->offsetBegin == buff->buff + buff->capacity) {
            buff->offsetBegin = buff->buff;
        }
        if (buff->offsetEnd == buff->buff + buff->capacity) {
            buff->offsetEnd = buff->buff;
        }
        if (buff->offsetEnd - buff->offsetBegin > 0) {
            int lenData = buff->offsetEnd - buff->offsetBegin;
            int numSym = write(buff->sendFd, buff->offsetBegin, lenData);
            fprintf(stderr, "%d send %d bytes\n", buff->rcvFd, numSym);
            if (numSym < 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            buff->offsetBegin += numSym;
            buff->size -= numSym;
            if (numSym < lenData) {
                break;
            }
        } else {
            int lenToEnd = buff->buff + buff->capacity - buff->offsetBegin;
            int numSym = write(buff->sendFd, buff->offsetBegin, lenToEnd);
            fprintf(stderr, "%d send %d bytes\n", buff->rcvFd, numSym);
            if (numSym < 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            buff->offsetBegin += numSym;
            buff->size -= numSym;
            if (numSym < lenToEnd) {
                break;
            }
        }
    }
}

void Child_Dead_Handler(int signum) {
    static size_t numChildrenDied = 0;
    static size_t numChild = 0;
    if (signum < 0) {
        numChild = abs(signum);
        fprintf(stdout, "NumChild = %d\n", numChild);
        return;
    }
    int status;
    wait(&status);
    if (WEXITSTATUS(status) != EXIT_SUCCESS) {
        fprintf(stderr, "Child died unexpectedly\n");
        exit(EXIT_FAILURE);
    }
    numChildrenDied++;
    fprintf(stdout, "Numchildre++ = %d\n", numChildrenDied);
    if (numChildrenDied == numChild) {
        exit(EXIT_SUCCESS);
    }
}

void Print_All_Buffers(struct Connection_t* buffs, size_t num) {
    fprintf(stderr, "------------------------------------------------\n");
    for (int i = 0; i < num; ++i) {
        fprintf(stderr, "rcvFd - %d, sendFd - %d, size - %zu, capacity - %zu\n", buffs[i].rcvFd, buffs[i].sendFd, buffs[i].size, buffs[i].capacity);
        fprintf(stderr, "Buff: [%s]\n", buffs[i].buff);
    }
    fprintf(stderr, "------------------------------------------------\n");
}
