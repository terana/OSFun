#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "msg.h"

#define CLIENT_ADDR 1
#define CODE 2

int recvMsg(rpc_addr_t addr, msg_t* msg) {
	if (msg == NULL || msg->param == NULL) {
		return -1;
	}
	sleep(2);
	msg->procedureCode = CODE;
	strcpy(msg->param, "Hello!");
	msg->paramLen = sizeof("Hello!");
	return 0;
}

void listenForConnect() {
	msg_t msg;
	msg.param = malloc(sizeof(MAX_PARAM_LEN));
	int res;
	while (1) {
		res = recvMsg(CLIENT_ADDR, &msg);
		if (res != 0) {
			printf("%s\n", "Error recieving msg\n");
			return;
		}
		switch (msg.procedureCode){
			case CODE: 
				printf("%s\n", msg.param);
				break;
			default:
				printf("%s\n", "Recieved invalid msg\n");
		}
	}
	free(msg.param);
}