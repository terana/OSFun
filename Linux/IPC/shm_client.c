#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

#include "prod_cons.h"

int Produce(int semid, char *shmem, int src_fd) {
	int n;
	int res;
	do {
		res = ProducerEnter(semid);
		if (res != 0) {
			return res;
		}
		n = read(src_fd, shmem, BSIZE);
		res = ProducerExit(semid);
		if (res != 0) {
			return res;
		}
	} while (n > 0);
	return n;
}

int main() {
	char *shmem = GetShMem(BSIZE);
	if (shmem == NULL) {
		perror("Can't get shared memory: ");
		exit(1);
	}

	int semid = GetSemID();
	if (semid < 0) {
		perror("Can't get semaphores: ");
		exit(1);
	}

	int res = ProducerInitSems(semid);
	if (res != 0) {
		perror("Can't init sems: ");
		exit(1);
	}

	int src_fd = open("src", O_RDONLY);
	if (src_fd < 0) {
		perror("Can't open source file:");
		exit(1);
	}

	res = Produce(semid, shmem, src_fd);
	if (res < 0) {
		perror("Error while producing: ");
		exit(1);
	}
}