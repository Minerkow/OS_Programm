#include "Writer.h"
#include "Reader.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {

    if (argc == 1) {
        Reader();
    }
    if (argc == 2) {
        Writer(argv[1]);
    }

    if (argc > 2) {
        fprintf(stderr, "Extra Arguments");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}