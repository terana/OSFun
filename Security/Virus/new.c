#include <stdio.h>

int main () {
	printf("%s\n", "Enter your string:");
	char buf[4];
	gets(buf);
	printf("%s\n", buf);
}