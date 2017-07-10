CFLAGS=  -g -Wall -std=c99

SOURCES=recipient-invoked.c task_list.c
TARGET=run

$(TARGET): $(SOURCES:.c=.o)
	$(CC) $(CFLAGS)   $^ -o $@

clean:
	rm -f *.o run*