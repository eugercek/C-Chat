#include "shared.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sysexits.h>
#include <unistd.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
  struct addrinfo hints, *server;
  int sockfd;
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
  if ((sockfd = socket(server->ai_family, server->ai_socktype,
                       server->ai_protocol)) == -1) {
    error_exit("socket", EX_UNAVAILABLE);
  }

  if (connect(sockfd, server->ai_addr, server->ai_addrlen) == -1)
    error_exit("connect", EX_UNAVAILABLE);

  strcpy(buffer, "Hello");
  freeaddrinfo(server);

  while (1) {
    scanf("%s", buffer);
    if (send(sockfd, buffer, BUFFER_SIZE, 0) < 0)
      error_exit("send", EX_UNAVAILABLE);
  }
  close(sockfd);
  return 0;
}
