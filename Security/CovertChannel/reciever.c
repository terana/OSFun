#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


#define BUF_SIZE 1000
#define ACCURACY 2

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

int main() {
	
	int src_fd;
	int dst_fd;
	openFiles(&src_fd, &dst_fd);
	time_t time_0 = copy(src_fd, dst_fd);
	printf("Time0 %ld\n", time_0);
	time_t time;
	int msg = 0;
	int sync = 1;

	//READY TO RECIEVE
	int ind_fd = open("rcv_indicator", O_WRONLY | O_CREAT, 0777);
	if (ind_fd < 0) {
		perror("Can't open rcv_inducator file:");
		exit(1);
	}

	while (sync != 0) {
		sync = fchmod(ind_fd, 0777);
		if(sync < 0) {
			perror("Can't set access: ");
			exit(1);
		}
		printf("Set mode 0777\n");
		close(src_fd);
		close(dst_fd);
		openFiles(&src_fd, &dst_fd);
		time = copy(src_fd, dst_fd);
		printf("Time %ld\n", time);
		if ((time < time_0 + ACCURACY) && (time > time_0 - ACCURACY)) {
			msg = msg << 1;
			printf("0\n");
			fflush(stdout);
		} else {
			msg |= 1;
			msg = msg << 1;
			printf("1\n");
			fflush(stdout);
		}
		printf("Set mode 0666\n");
		sync = fchmod(ind_fd, 0666);
		if(sync < 0) {
			perror("Can't set access: ");
			exit(1);
		}
		sleep(1);
		sync = -1;
		sync = access("snd_indicator", X_OK);
	}
	msg = msg >> 1;
	printf("\n%d\n", msg);
	close(src_fd);
	close(dst_fd);
}