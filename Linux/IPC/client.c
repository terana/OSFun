#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERV_PATH "serv_sock"

extern void copy (int src_fd, int dst_fd);


int main () {
	int sock_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		perror("Can't create socket: ");
		exit(1);
	}

	struct sockaddr_un serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sun_family = PF_LOCAL;
	strcpy(serv_addr.sun_path, SERV_PATH);

	if (connect(sock_fd, (struct sockaddr *) &serv_addr,  sizeof(serv_addr)) < 0) {
		perror("Can't connect to server: ");
		close(sock_fd);
		exit(1);
	}

	int src_fd = open("src", O_RDONLY);
	if (src_fd < 0) {
		perror("Can't open source file:");
		exit(1);
	}

	copy(src_fd, sock_fd);

	close(sock_fd);
	close(src_fd);
}