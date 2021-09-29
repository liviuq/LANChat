CC=gcc
CFLAGS=-Wall -Werror -O3

all: client server

server: 
	$(CC) $(CFLAGS) source/server.c -o ~/Documents/LANChat/bin/server

client:
	$(CC) $(CFLAGS) source/client.c -o ~/Documents/LANChat/bin/client

.PHONY: clean

clean:
	rm bin/server bin/client