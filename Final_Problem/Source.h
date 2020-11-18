#pragma once

#define _GNU_SOURCE

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

enum {TIMEOUT = 1};

struct Connection_t {
    int rcvPipe[2];
    int sendPipe[2];
};
long long int Read_Number_from_Text(const char* text);
void Child_Run(int sendFd, int rcvFd);
void Parent_Run();
void Loader_Run(char* path, int sendFd);