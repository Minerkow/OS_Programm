#include "Writer.h"
#include "Reader.h"

int main(int argc, char** argv) {

    if (argc == 1) {
        Get_from_Writer();
    }
    if (argc == 2) {
        Send_to_Reader(argv[1]);
    }

    if (argc > 2) {
        fprintf(stderr, "Extra Arguments");
        exit(ERROR);
    }
    return 0;
}