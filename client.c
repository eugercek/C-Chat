#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct addrinfo hints, *server;
  int sockid;

  if (argc != 2) {
    fprintf(stderr, "Usage : %s port_no\n", argv[0]);
    exit(1);
  }

  // TODO Initializer vs memset
  // TODO Should macro?
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  int ret;
  if ((ret = getaddrinfo(NULL, argv[1], &hints, &server)) != 0) {
    if (ret == EAI_SYSTEM)
      perror("getaddrinfo(): ");
    else
      fprintf(stderr, "getaddrinfo: %s", gai_strerror(ret));
    exit(2);
  }

  // TODO Create loop for ipv6
  if ((sockid = socket(server->ai_family, server->ai_socktype,
                       server->ai_protocol)) == -1) {
    perror("socket(): ");
    exit(3);
  }

  if (connect(sockid, server->ai_addr, server->ai_addrlen) == -1) {
    perror("connect(): ");
    exit(3);
  }
  freeaddrinfo(server);

  /* const char *message = "Client: Hello Server!\n"; */
  /* send(sockid, message, strlen(message), 0); */
  close(sockid);
  return 0;
}
