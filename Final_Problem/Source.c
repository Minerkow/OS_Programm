#include "Source.h"

long long int Read_Number_from_Text(const char* text) {
    char **endptr;
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
    while (1) {
        int numSymbols = splice( rcvFd, NULL,
                                 sendFd, NULL,
                                 PIPE_BUF, SPLICE_F_MOVE);
        if (numSymbols < 0) {
            perror("splice()");
            exit(EXIT_FAILURE);
        }
        if (numSymbols == 0)
            break;
    }
}

void Loader_Run(char* path, int sendFd) {
    int fileFd = open(path, O_RDONLY);
    while (1) {

        int numSymbols = splice( fileFd, NULL,
                                 sendFd, NULL,
                                 PIPE_BUF, SPLICE_F_MOVE);
        if (numSymbols < 0) {
            perror("splice()");
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
            buff->offsetBegin += numSym;
            buff->size += numSym;
            if (numSym < lenData) {
                break;
            }
        } else {
            int lenToEnd = buff->buff + buff->capacity - buff->offsetBegin;
            int numSym = write(buff->sendFd, buff->offsetBegin, lenToEnd);
            buff->offsetBegin += numSym;
            buff->size += numSym;
            if (numSym < lenToEnd) {
                break;
            }
        }
}