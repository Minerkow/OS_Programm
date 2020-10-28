#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "reader.h"

void Writer(const char* path, pid_t rpid);

void Confirm_Handler(int sig);