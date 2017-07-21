#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define SERV_PATH "serv_sock"

extern void copy (int src_fd, int dst_fd);

int main() {
	int sock_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		perror("Can't create socket: ");
		exit(1);
	}

	struct sockaddr_un serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sun_family = PF_LOCAL;
	strcpy(serv_addr.sun_path, SERV_PATH);
	if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Can't bind socket: ");
		close(sock_fd);
		exit(1);
	}
	if (listen(sock_fd, 5) < 0) {
		perror("Error in listen: ");
		close(sock_fd);
		exit(1);
	}

	struct sockaddr_un cli_addr;
	socklen_t clilen = sizeof(cli_addr);
	int det_sock_fd = accept(sock_fd, (struct sockaddr *) &cli_addr, &clilen);
	if (det_sock_fd < 0) {
		perror("Error in accept: ");
		close(sock_fd);
		exit(1);
	}

	int dst_fd = open("dst", O_WRONLY | O_CREAT, 0777);
	if (dst_fd < 0) {
		perror("Can't open destination file:");
		close(sock_fd);
		exit(1);
	}

	copy(det_sock_fd, dst_fd);

	close(det_sock_fd);
	close(sock_fd);
	close(dst_fd);
}