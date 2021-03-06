#pragma once

#define _GNU_SOURCE

#define $(ret)                    \
    if (ret < 0) {                \
        perror (#ret);            \
        fprintf(stderr, "ERROR"); \
        exit(EXIT_FAILURE);       \
    }

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <errno.h>
#include <assert.h>
#include <poll.h>

enum {TIMEOUT = 1, READ = 0, WRITE = 1};

struct Connection_t {
    int rcvFd;
    int sendFd;
    char* buff;

    size_t capacity;
    size_t size;

    char* offsetBegin;
    char* offsetEnd;
};

struct Ufds_t {
    struct pollfd *data;
    size_t* buffsInd;
    size_t size;
    size_t notCloseIndex;
};

long long int Read_Number_from_Text(const char* text);
void Child_Run(int sendFd, int rcvFd);
void Parent_Run();
void Loader_Run(char* path, int sendFd);

size_t Size_Buf(size_t degree);

void Load_To_Buff(struct Connection_t* buff);
void Download_From_Buff(struct Connection_t* buff);

void Child_Dead_Handler(int signum);

void Print_All_Buffers(struct Connection_t* buffs, size_t num);
