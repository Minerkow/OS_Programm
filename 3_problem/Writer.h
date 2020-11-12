#pragma once

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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

enum {PROJ_ID = 5, SHMSIZE = 4096, DATASIZE = SHMSIZE - sizeof(size_t)};
enum {MUTEX = 0, EMPTY = 1, FULL = 2, WRITER = 3, READER = 4, CONNECT = 5};
enum {P = -1, V = 1, W = 0};

struct Data_t {
    size_t numSymbols_;
    char data_[DATASIZE];
};

void Writer(char* path);
void SemOp(int sem, short sem_op, int sem_num, short flag);
int InitSem(key_t key, unsigned short* value, int numSem);
void DeleteSem(int semId);

//void ReadySemOp(int readySem, int sem_num, short sem_op);
//int InitSem(key_t key, int numSem);