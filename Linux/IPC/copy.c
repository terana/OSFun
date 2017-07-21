#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BSIZE 1000

void copy (int src_fd, int dst_fd) {
	char* buf = malloc(BSIZE * sizeof(char));
	if (buf == NULL) {
		perror("Can't create buffer: ");
		exit(1);
	}

	int n;
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
	free(buf);
}