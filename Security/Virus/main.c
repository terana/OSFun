#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define BUF_SIZE 100

int main() {
	int new_fd = open("new", O_RDONLY);
	if (new_fd < 0){
		perror("Can't open new file: ");
		exit(1);
	}
	int old_fd = open("old", O_WRONLY);
	if (old_fd < 0){
		perror("Can't open old file: ");
		exit(1);
	}

	int n;
	char* buf = malloc(BUF_SIZE * sizeof(char));
	do {
		n = read(new_fd, buf, BUF_SIZE);
		if (n < 0) {
			perror("Can't read from file: ");
			exit(1);
		}
		n = write(old_fd, buf, n);
		if (n < 0) {
			perror("Can't write to file:");
			exit(1);
		}
	} while (n > 0);
	free(buf);
	close(new_fd);
	close(old_fd);

}