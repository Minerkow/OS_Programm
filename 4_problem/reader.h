#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

enum {ZERO_SIGNAL = SIGUSR1, ONE_SIGNAL = SIGUSR2, CONFIRM = SIGUSR1};

void Reader(pid_t writerPid);

void One_Handler(int sig);
void Zero_Handler(int sig);
void Dead_Child_Handler(int sig);