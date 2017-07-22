CFLAGS=  -Wall -std=c99

tcp_client:
	$(CC) $(CFLAGS) -o tcp_client tcp_client.c copy.c
tcp_server:
	$(CC) $(CFLAGS) -o tcp_server tcp_server.c copy.c
shm_client:
	$(CC) $(CFLAGS) -o shm_client shm_client.c prod_cons.c
shm_server:
	$(CC) $(CFLAGS) -o shm_server shm_server.c prod_cons.c
clean:
	rm -f *.o tcp_client tcp_server *sock shm_client shm_server