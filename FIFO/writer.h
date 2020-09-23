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

enum {BLOCK_SIZE = 10, ERROR = -1};

void Load_from_File_in_Fifo(char way[]);

