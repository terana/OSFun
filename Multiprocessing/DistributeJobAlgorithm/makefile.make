CFLAGS=  -g -Wall -std=c++11

SOURCES=main.cpp rights.cpp
TARGET=run

$(TARGET): $(SOURCES:.c=.o)
	$(CC) $(CFLAGS)   $^ -o $@

clean:
	rm -f *.o run*