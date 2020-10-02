#pragma once

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
#include <time.h>

#include "Writer.h"

enum {WRITER_TIMEOUT = 5, WAITING_WRITER_TIMEOUT = 0};

void Get_from_Writer();

void Connect_with_Writer(int fdServ);
void Get_File(int fdData);
int Open_Data_Fifo(int fdData);

