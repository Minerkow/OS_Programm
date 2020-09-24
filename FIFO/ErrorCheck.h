#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum{ERROR = -1};

void PerrorCheck(bool key, char *nameFunc);

void ErrorCheck(bool key, char *message);

