#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "prod_cons.h"

int Consume(int semid, char *shmem, int dst_fd) {
	int n;
	int res;
	do {
		res = ConsumerEnter(semid);
		if (res != 0) {
			return res;
		}
		n = write(dst_fd, shmem, BSIZE);
		res = ConsumerExit(semid);
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

	int res = ConsumerInitSems(semid);
	if (res != 0) {
		perror("Can't init sems: ");
		exit(1);
	}

	int dst_fd = open("dst", O_WRONLY | O_CREAT, 0777);
	if (dst_fd < 0) {
		perror("Can't open destination file:");
		exit(1);
	}

	res = Consume(semid, shmem, dst_fd);
	if (res < 0) {
		perror("Error while consuming: ");
		exit(1);
	}	
}