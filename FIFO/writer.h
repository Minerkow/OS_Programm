#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <signal.h>

#include "reader.h"

enum {
    DATA_SIZE = PIPE_BUF - sizeof(pid_t) * 2 - sizeof(unsigned) - 1,
    CONNECT = -1
};

struct Message_t {
    pid_t pidReader;
    pid_t pidWriter;
    unsigned size;
    char data[DATA_SIZE];
};

void Load_from_File_in_Fifo(char way[]);

void Connect_with_Reader(int fdData, int fdAns, pid_t* pidReader);

bool Send_Message(int fdFile, int fdData, pid_t pidReader);