#include "writer.h"
#include "reader.h"

int main(int argc, char** argv) {

    if (argc == 1) {
        Load_from_Fifo_in_Stdin();
    }
    if (argc == 2) {
        Load_from_File_in_Fifo(argv[1]);
    }
    if (argc > 2) {
        fprintf(stderr, "Extra Arguments");
        exit(ERROR);
    }
    return 0;
} 