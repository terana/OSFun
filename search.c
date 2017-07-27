#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>

#define STR_LEN 5
#define ARR_LEN 50000000
#define SEARCH_TIMES 10

char * BrudeForceSearch(char** arr, int len, char* str) {
	for (int i = 0; i < len; i++) {
		if (!strcmp(str, arr[i])) {
			return arr[i];
		}
	}
	return NULL;
}

static int sorted = 0;

int compar(const void* v1, const void* v2) {
	char* s1 = (char *) v1;
	char* s2 = (char *) v2;
	return strcmp(s1, s2);
}

char* BinSearch(char** arr, int len, char* str) {
	int rest = len % 2;
	len /= 2;
	int cmp = strcmp(arr[len], str);
	if (cmp == 0) 
		return arr[len];
	if (len == 0) 
		return NULL;
	return cmp < 0 ? BinSearch(&(arr[len + 1]), len - 1 + rest, str) : BinSearch(arr, len, str);
}

char* SortSearch(char** arr, int len, char* str) {
	if (sorted == 0) {
		qsort(arr, len, sizeof(char*), compar);
		sorted = 1;
	}
	return BinSearch(arr, len, str);
}

double TimeIntvlInMiliSec(struct rusage * begin, struct rusage * end) {
	int s_sec = end->ru_stime.tv_sec - begin->ru_stime.tv_sec;
	int s_usec = end->ru_stime.tv_usec - begin->ru_stime.tv_usec;
	double s_dif = s_sec*1000 + (double) s_usec/1000;

	int u_sec = end->ru_utime.tv_sec - begin->ru_utime.tv_sec;
	int u_usec = end->ru_utime.tv_usec - begin->ru_utime.tv_usec;
	double u_dif = u_sec*1000 + (double) u_usec/1000;
	return u_dif + s_dif;
}


int main () {
	int fd = open("/dev/random", O_RDONLY);
	if (fd < 0) {
		perror("Can't open /dev/random: ");
		exit(1);
	}
	
	char** arr = malloc(ARR_LEN * sizeof(char*));
	int n;
	for (int i = 0; i < ARR_LEN; i++) {
		arr[i] = malloc(STR_LEN * sizeof(char));
		n = read(fd, arr[i], STR_LEN);
		if(n < 0) {
			perror("Can't read from /dev/random: ");
			exit(1);
		}
	}

	char* str [SEARCH_TIMES];
	int r;
	for (int i = 0; i < SEARCH_TIMES; i++) {
		r = random() % ARR_LEN;
		str[i] = arr[r];
	}

	struct rusage begin, end;
	int ret = getrusage(RUSAGE_SELF, &begin);
	if (ret != 0) {
		perror("Can't get recource usage: ");
		exit(0);
	}
	
	for (int i = 0; i < SEARCH_TIMES; i++) {
		BrudeForceSearch(arr, ARR_LEN, str[i]);
	}
	
	ret = getrusage(RUSAGE_SELF, &end);
	if (ret != 0) {
		perror("Can't get recource usage: ");
		exit(0);
	}
	
	printf("brudeforce search time total: %lf msec\n",  TimeIntvlInMiliSec(&begin, &end) / SEARCH_TIMES);
	
	ret = getrusage(RUSAGE_SELF, &begin);
	if (ret != 0) {
		perror("Can't get recource usage: ");
		exit(0);
	}
	
	for (int i = 0; i < SEARCH_TIMES; i++) {
		SortSearch(arr, ARR_LEN, str[i]);
	}

	ret = getrusage(RUSAGE_SELF, &end);
	if (ret != 0) {
		perror("Can't get recource usage: ");
		exit(0);
	}
	
	printf("sort search time total: %lf msec\n",  TimeIntvlInMiliSec(&begin, &end) / SEARCH_TIMES);

	for (int i = 0; i < ARR_LEN; i++) {
		free(arr[i]);
	}
	free(arr);
	close(fd);
}