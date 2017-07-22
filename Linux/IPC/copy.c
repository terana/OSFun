#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "prod_cons.h"

#define BSIZE 1000

long copy (int src_fd, int dst_fd) {
	char* buf = malloc(BSIZE * sizeof(char));
	if (buf == NULL) {
		perror("Can't create buffer: ");
		exit(1);
	}

	int n;
	time_t begin = time(NULL);
	do {
		n = read(src_fd, buf, BSIZE);
		if (n < 0) {
			perror("Can't read from file:");
			exit(1);
		}
		n = write(dst_fd, buf, n);
		if (n < 0) {
			perror("Can't write to file:");
			exit(1);
		}
	} while (n > 0);
	time_t end = time(NULL);
	free(buf);
	return end - begin;
}