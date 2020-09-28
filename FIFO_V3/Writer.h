#pragma once

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <signal.h>


enum {
    ERROR = -1,
    MAX_LEN_FIFO_NAME = 50,
    READER_TIMEOUT = 1,
    WAITING_READER_TIMEOUT = 2
};

void Send_to_Reader(char path[]);
void Connect_with_Reader(char dataFifo[], int fdServ, pid_t* pidReader);
void Send_File(int fdFile, int fdData, pid_t pidReader);
