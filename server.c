#include "shared.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sysexits.h> // For Meaningful return values
#include <unistd.h>

#define BACKLOG 20
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
  struct addrinfo hints, *server;
  int sockfd;
  char buffer[BUFFER_SIZE];

  if (argc != 2) {
    fprintf(stderr, "Usage : %s port_no\n", argv[0]);
    exit(EX_USAGE);
  }

  // TODO Should macro?
  memzero(&hints, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int ret;
  if ((ret = getaddrinfo(NULL, argv[1], &hints, &server)) != 0) {
    if (ret == EAI_SYSTEM)
      perror("getaddrinfo(): ");
    else
      fprintf(stderr, "getaddrinfo: %s", gai_strerror(ret));
    exit(EX_UNAVAILABLE);
  }

  // TODO Create loop for ipv6
  if ((sockfd = socket(server->ai_family, server->ai_socktype,
                       server->ai_protocol)) == -1) {
    error_exit("socket", EX_UNAVAILABLE);
  }

  // THE Most impotant funciton in this code
  // If you don't use this you can't bind same port number
  // untill TIME_WAIT time. Terrible for debug
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    error_exit("setsockopt", EX_UNAVAILABLE);

  if (bind(sockfd, server->ai_addr, server->ai_addrlen) == -1)
    error_exit("bind", EX_UNAVAILABLE);

  freeaddrinfo(server);

  if (listen(sockfd, BACKLOG) == -1)
    error_exit("listen", EX_UNAVAILABLE);

  int client;
  struct sockaddr_storage client_addr;
  char client_ip_string[INET6_ADDRSTRLEN];
  socklen_t addrlen = sizeof client_addr;

  while (1) {
    client = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen);
    if (client == -1)
      error_exit("accept", EX_UNAVAILABLE);

    // TODO Better way to give addr
    inet_ntop(client_addr.ss_family,
              &((struct sockaddr_in *)&client_addr)->sin_addr, client_ip_string,
              sizeof client_ip_string);
    printf("Connection : %s\n", client_ip_string);

    while (1) {
      if (recv(client, buffer, BUFFER_SIZE, 0) < 0)
        error_exit("recv", EX_UNAVAILABLE);
      printf("> %s\n", buffer);
    }
  }
  close(sockfd);
  return 0;
}
