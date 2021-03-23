##
# C Socket Server
#
# @file
# @version 0.1

# TODO Think about autotools

CC = gcc
CFLAGS = -Wall -Wextra -g


# TODO Clean Makefile

all: server client

server: server.o shared.o
client: client.o shared.o


.PHONY: clean
clean:
	rm -rf $(OBJS)
	rm -rf $(TARGETS)
# end
