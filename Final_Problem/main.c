#include "Source.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Expected Argument");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "%s", argv[1]);
    long long int numChild = Read_Number_from_Text(argv[1]);
    if (argc > 3) {
        fprintf(stderr, "Extra Arguments");
        exit(EXIT_FAILURE);
    }

    assert(argv[2]);

    struct Connection_t* connectArr = calloc(numChild, sizeof(struct Connection_t));
    for (long long int i = 0; i < numChild; ++i) {
        pipe(connectArr[i].rcvPipe);
        pipe(connectArr[i].sendPipe);

        pid_t pid = fork();
        if (i == 0 && pid == 0) {
            Loader_Run(argv[2], connectArr[i].rcvPipe[1]);
            close(connectArr[i].sendPipe[0]);
            close(connectArr[i].sendPipe[1]);
            continue;
        }
        if (pid == 0) {
            Child_Run(connectArr[i].rcvPipe[1], connectArr[i].sendPipe[0]);
        }
    }

    for (long long int i = 0; i < numChild++; ++i) {
        
    }

    return 0;
}
