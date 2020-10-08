#pragma once

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

struct msgBuff_t {
    long mtype_;
    long long id_;
};

long long int Read_Number_from_Bash (int argc, char** argv);
void Create_Children(long long int num);
void Handler(int signum);
void Free_Msg(int id);