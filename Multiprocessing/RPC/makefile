CFLAGS=  -g -Wall -std=c99

SOURCES=RPCServerStub.c RPCClientStub.c test.c
TARGET=test

$(TARGET): $(SOURCES:.c=.o)
	$(CC) $(CFLAGS)   $^ -o $@

test: test.o RPCServerStub.o RPCClientStub.o
	$(CC) $(CFLAGS)   $^ -o $@

clean:
	rm -f *.o test*