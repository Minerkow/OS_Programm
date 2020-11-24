#include "Header.h"

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
#ifdef DEBUG
    fprintf(stderr, "I download from %d to %d\n", rcvFd, sendFd);
#endif
    while (1) {
        //fprintf(stderr, "Splice\n");
        int numSymbols = splice( rcvFd, NULL,
                                 sendFd, NULL,
                                 PIPE_BUF, SPLICE_F_MOVE);
        //fprintf(stderr, "From splice\n");
        if (numSymbols < 0) {
            perror("splice()");
            fprintf(stderr, "ERROR in fd - %d, %d", rcvFd, sendFd);
            exit(EXIT_FAILURE);
        }
        if (numSymbols == 0) {
#ifdef DEBUG
            fprintf(stderr, "Exit Success\n");
#endif
            fflush(stdout);
            close(rcvFd);
            close(sendFd);
            exit(EXIT_SUCCESS);
        }
    }
}

void Loader_Run(char* path, int sendFd) {
    int fileFd = open(path, O_RDONLY);
    $(fileFd)
#ifdef DEBUG
    fprintf(stderr, "I download from fileFd: %d to %d\n", fileFd, sendFd);
#endif
    while (1) {

        int numSymbols = splice( fileFd, NULL,
                                 sendFd, NULL,
                                 PIPE_BUF, SPLICE_F_MOVE);
#ifdef DEBUG
        fprintf(stderr, "Loader send %d bytes\n", numSymbols);
#endif
        if (numSymbols < 0) {
            perror("splice");
            fprintf(stderr, "ERROR in fd - fileFD - %d, %d", fileFd, sendFd);
            exit(EXIT_FAILURE);
        }
        if (numSymbols == 0) {
            fflush(stdout);
            close(sendFd);
            close(fileFd);
#ifdef DEBUG
            fprintf(stderr, "Loader exit\n");
#endif
            exit(EXIT_SUCCESS);
        }
    }
}

size_t Size_Buf(size_t degree) {
    int size = 1;
    for (size_t i = 0; i < degree; ++i) {
        size *= 3;
    }
    return size * 1024;
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
            perror("read ERROR");
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
            int numSym = read(buff->rcvFd, buff->offsetEnd, lenToEnd);
            if (numSym < 0) {
                perror("read ERROR");
                exit(EXIT_FAILURE);
            }
            buff->size += numSym;
            buff->offsetEnd += numSym;

            if (numSym < lenToEnd) {
                return;
            }

        } else {
            size_t freeData = buff->offsetBegin - buff->offsetEnd;
            assert(freeData == buff->capacity - buff->size);
            int numSym = read(buff->rcvFd, buff->offsetEnd, freeData);
            if (numSym < 0) {
                perror("read ERROR");
                exit(EXIT_FAILURE);
            }
            buff->size += numSym;
            assert(buff->size <= buff->capacity);
            buff->offsetEnd += numSym;
            if (numSym <= freeData) {
                return;
            }
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
#ifdef DEBUG
            fprintf(stderr, "%d send %d bytes\n", buff->rcvFd, numSym);
#endif
            if (numSym < 0) {
                if (errno == EAGAIN) {
                    return;
                }
                perror("write1 ERROR");
                fprintf(stderr, "%zu/%zu fd - %d", buff->size, buff->capacity, buff->sendFd);
                exit(EXIT_FAILURE);
            }
            buff->offsetBegin += numSym;
            buff->size -= numSym;
            assert(buff->size >= 0);
            if (numSym <= lenData) {
                return;
            }
        } else {
            int lenToEnd = buff->buff + buff->capacity - buff->offsetBegin;
            int numSym = write(buff->sendFd, buff->offsetBegin, lenToEnd);
#ifdef DEBUG
            fprintf(stderr, "%d send %d bytes\n", buff->rcvFd, numSym);
#endif
            if (numSym < 0) {
                if (errno == EAGAIN) {
                    return;
                }
                perror("write2 ERROR");
                exit(EXIT_FAILURE);
            }
            buff->offsetBegin += numSym;
            buff->size -= numSym;
            if (numSym < lenToEnd) {
                return;
            }
        }
    }
}

void Child_Dead_Handler(int signum) {
}

void Print_All_Buffers(struct Connection_t* buffs, size_t num) {
    fprintf(stderr, "------------------------------------------------\n");
    for (int i = 0; i < num; ++i) {
        fprintf(stderr, "rcvFd - %d, sendFd - %d, size - %zu, capacity - %zu\n", buffs[i].rcvFd, buffs[i].sendFd, buffs[i].size, buffs[i].capacity);
        fprintf(stderr, "Buff: [");
        for (int j = 0; j < buffs[i].capacity; ++j) {
            if (buffs[i].buff[j] == 0)
                continue;
            fprintf(stderr, "%c", buffs[i].buff[j]);
        }
        fprintf(stderr, "]\n");
    }
    fprintf(stderr, "------------------------------------------------\n");
}
