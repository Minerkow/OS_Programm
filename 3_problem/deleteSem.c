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
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int main() {
	key_t key = ftok("/tmp", 5);
	int semId = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
	if (semctl(semId, 0, IPC_RMID) < 0) {
		perror("semctl()");
		exit(-1);
	}
	return 1;
}