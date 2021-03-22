##
# C Socket Server
#
# @file
# @version 0.1

# TODO Think about autotools

CC=gcc
CFLAGS= -Wall -Wextra

all: server client

server: server.o
	$(CC) $(CFLAGS) server.o -o server.out
server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client: client.o
	$(CC) $(CFLAGS) client.o -o client.out
client.o: client.c
	$(CC) $(CFLAGS) -c client.c

.PHONY: clean
clean:
	rm -rf $(wildcard *.o)

.PHONY: run

# end
