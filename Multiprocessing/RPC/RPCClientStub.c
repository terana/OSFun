#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "msg.h"

#define SERVER_ADDR 1
#define CODE 2

int argsAreValid(const char* format) {
	return 1;
}

void marshal(const char* format, int val, char* param, int paramLen) {
	char *occ = strstr(format, "%d");
	
	int firstPartLen = strlen(format) - strlen(occ);
	if (occ == NULL || !strcmp(occ, format) || firstPartLen >= paramLen) {
		strncpy(param, format, paramLen);
		return;
	} else {
		strncpy(param, format, firstPartLen);
		int valLen = sprintf(param + firstPartLen, "%d", val);
		strncpy(param + firstPartLen + valLen, occ + 2, paramLen - firstPartLen - valLen);
	}

}

int sendMsg(rpc_addr_t addr, msg_t *msg, int msgLen){
	return 0;
}

int RPCprintf(const char * restrict format, int val) {
	if (argsAreValid(format) != 1) {
		return -1;
	}
	char *param = malloc(MAX_PARAM_LEN);
	marshal(format, val, param, MAX_PARAM_LEN);
	printf("param from Client: %s\n", param);
	msg_t msg;
	msg.procedureCode = CODE;
	msg.param = (void *) param; 
	msg.paramLen = sizeof(param);

	sendMsg(SERVER_ADDR, &msg, sizeof(msg));
	free(param);
	return 0;
}