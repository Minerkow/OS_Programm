#include "ErrorCheck.h"

void PerrorCheck(bool key, char *nameFunc) {
    if (key) {
        perror(nameFunc);
        exit(ERROR);
    }
}

void ErrorCheck(bool key, char *message) {
    if (key) {
        fprintf(stderr, "%s", message);
        exit(ERROR);
    }
}