CFLAGS=  -Wall -std=c99

client:
	$(CC) $(CFLAGS) -o client client.c copy.c
server:
	$(CC) $(CFLAGS) -o server server.c copy.c
clean:
	rm -f *.o client server