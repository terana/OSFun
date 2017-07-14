#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define BUF_SIZE 1000
#define N_CHILD 10


time_t copy (int src_fd, int dst_fd) {
	int n;
	char* buf = malloc(BUF_SIZE * sizeof(char));
	time_t begin = time(NULL);

	do {
		n = read(src_fd, buf, BUF_SIZE);
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

void openFiles(int* src_fd, int* dst_fd) {
	*src_fd = open("src", O_RDONLY);
	if (*src_fd < 0) {
		perror("Can't open source file:");
		exit(1);
	}

	*dst_fd = open("dst", O_WRONLY | O_CREAT, 0777);
	if (*dst_fd < 0) {
		perror("Can't open destination file:");
		exit(1);
	}
}

int main ()
{
	int src_fd;
	int dst_fd;
	openFiles(&src_fd, &dst_fd);

	time_t time = copy(src_fd, dst_fd);
	printf("Time of copying without extra work: %ld sec\n", time);

	close(src_fd);
	close(dst_fd);

	openFiles(&src_fd, &dst_fd);
	int pids[N_CHILD];
	for (int i = 0; i < N_CHILD; i++) {
		pids[i] = fork();
		//CHILD
		if (pids[i] == 0) { 
			while(1) {};
		}

		//PARENT
		if (pids[i] < 0) {
			perror("Can't create childprocess: ");
			exit(1);
		}
	}

	time = copy(src_fd, dst_fd);
	printf("Time of copying with extra work: %ld sec\n", time);

	for (int i = 0; i < N_CHILD; i++) {
		kill(pids[i], 9);
	}

	close(src_fd);
	close(dst_fd);
	return 0;
}