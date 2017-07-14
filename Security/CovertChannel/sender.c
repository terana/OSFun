#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define MSG 9 //1001
#define BIT_TIME 6
#define N_CHILD 10

void send_1() {
	int pids[N_CHILD];
	for (int i = 0; i < N_CHILD; i++) {
		pids[i] = fork();
		//CHILD
		if (pids[i] == 0) { 
			while (1){}
		}

		//PARENT
		if (pids[i] < 0) {
			perror("Can't create child: ");
			exit(-1);
		}
	}
	int sync = 0;
	while (sync == 0) {
		sync = access("rcv_indicator", X_OK);
	}
	for (int i = 0; i < N_CHILD; i++) {
		kill(pids[i], 9);
	}
}

void send_0 () {
	int sync = 0;
	while (sync == 0) {
		sync = access("rcv_indicator", X_OK);
	}
}

int main() {
	int ind_fd = open("snd_indicator", O_WRONLY | O_CREAT, 0666);
	if (ind_fd < 0) {
		perror("Can't open send inducator file:");
		exit(1);
	}

	int sync;
	int msg = MSG;
	while (msg) {
		sync = -1;
		while (sync != 0) {
			sync = access("rcv_indicator", X_OK);
		}
		//SENDING
		if (msg % 2) {
			send_1();
			printf("1");
			fflush(stdout);
		} else {
			send_0();
			printf("0");
			fflush(stdout);
		}
		
		msg /= 2;
	}

	printf("\nSend msg %d\n", MSG);
	sync = fchmod(ind_fd, 0777);
	if(sync < 0) {
		perror("Can't set access: ");
		exit(1);
	}
	int res = remove("snd_indicator");
	if (res != 0) {
		perror("Can't remove file: ");
	}
	close(ind_fd);
	return 0;
}