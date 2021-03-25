#include "netutils.h"
#include "shared.h"

// Network
#include <arpa/inet.h>
#include <netdb.h>

// UNIX
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sysexits.h>
#include <unistd.h>

// stdlib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
#define MAX_USERNAME_SIZE 16

static int serverfd;

void *listen_server(void *arg) {
  char buffer[BUFFER_SIZE];
  while (1) {
    if (recv(serverfd, buffer, BUFFER_SIZE, 0) < 0)
      error_exit("recv", EX_UNAVAILABLE);
    printf("%s", buffer);
    fflush(stdout);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  struct addrinfo hints, *server;
  char buffer[BUFFER_SIZE];

  if (argc != 2) {
    fprintf(stderr, "Usage : %s port_no\n", argv[0]);
    exit(1);
  }

  // TODO Should macro?
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  int ret;
  if ((ret = getaddrinfo("127.0.0.1", argv[1], &hints, &server)) != 0) {
    if (ret == EAI_SYSTEM)
      perror("getaddrinfo(): ");
    else
      fprintf(stderr, "getaddrinfo: %s", gai_strerror(ret));
    exit(2);
  }

  // TODO Create loop for ipv6
  if ((serverfd = socket(server->ai_family, server->ai_socktype,
                         server->ai_protocol)) == -1) {
    error_exit("socket", EX_UNAVAILABLE);
  }

  if (connect(serverfd, server->ai_addr, server->ai_addrlen) == -1)
    error_exit("connect", EX_UNAVAILABLE);

  strcpy(buffer, "Hello");
  freeaddrinfo(server);

  // Welcome!\n Enter a username:
  if (recv(serverfd, buffer, BUFFER_SIZE, 0) < 0)
    error_exit("recv", EX_UNAVAILABLE);
  printf("%s", buffer);

  char username[MAX_USERNAME_SIZE];
  fgets(username, MAX_USERNAME_SIZE, stdin);
  remove_newline(username);

  if (send_all(serverfd, username, MAX_USERNAME_SIZE) == -1)
    error_exit("send_all", EX_UNAVAILABLE);

  pthread_t tid;
  pthread_create(&tid, NULL, listen_server, NULL);
  while (1) {
    fgets(buffer, BUFFER_SIZE, stdin);
    remove_newline(buffer);
    if (send_all(serverfd, buffer, BUFFER_SIZE) == -1)
      error_exit("send_all", EX_UNAVAILABLE);
  }
  close(serverfd);
  return 0;
}
