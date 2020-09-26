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

#include "Writer.h"

enum {WRITER_TIMEOUT = 1};

void Get_from_Writer();

void Connect_with_Writer(int fdServ);
void Get_File(int fdData);

