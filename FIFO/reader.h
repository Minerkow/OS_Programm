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

#include "writer.h"
#include "ErrorCheck.h"

struct ConnectAccept_t {
    pid_t pidReader;
    pid_t pidWriter;
};

void Load_from_Fifo_in_Stdin();

void Connect_with_Writer(int fdAns, int fdData, pid_t* pidWriter);
bool Receive_Message(int fdData, pid_t pidWriter);