#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 255
#define MAX_ARGS 10

int main() {
	char* input = malloc(MAX_LEN * sizeof(char));
	char **ap, *argv[MAX_ARGS];
	int pid;
	int res;

	while(1) {
		printf("%s","$: ");
		size_t linecapp = MAX_LEN;
		size_t sz = getline(&input, &linecapp, stdin);
		input[sz-1] = '\0';
		for (ap = argv; (*ap = strsep(&input, " \t")) != NULL;) {
			if (**ap != '\0') {
				if (++ap >= &argv[MAX_ARGS]) {
					printf("%s", "Too many arguments\n");
					break;
				}
			}
		}
		pid = fork();
		if (pid == 0) { //CHILD
			printf("command: %s, args: %s", argv[0], argv[1]);
			res = execvp(argv[0], argv);
			if (res != 0) {
				perror("Can't do the command: ");
				exit(1);
			}
		} else if (pid > 0) {
			waitpid(-1, &res, 0);
			if (WIFEXITED(res)){
				printf("%d", WIFEXITED(res));
			}
		} else {
			perror("Can't create process: ");
			exit(1);
		}
	}
}