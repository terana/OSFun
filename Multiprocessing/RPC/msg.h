#ifndef MSG
#define MSG

#define MAX_PARAM_LEN 50

typedef  int rpc_addr_t;

typedef struct {
	int procedureCode;
	void *param;
	int paramLen;
} msg_t;

#endif