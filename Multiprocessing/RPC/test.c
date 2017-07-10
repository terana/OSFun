#include "RPCClientStub.h"
#include "RPCServerStub.h"
#include <stdio.h>

int main() {
	if (RPCprintf("See you %d\n", 5) != 0) {
		printf("%s\n", "Error calling RPCprintf");
		return -1;
	}

	listenForConnect();
	return 0;
}