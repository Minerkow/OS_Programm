#include "msg.h"

int main(int argc, char** argv) {
    long long int num = Read_Number_from_Bash(argc, argv);
    Create_Children(num);
    return 0;
}
