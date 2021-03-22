#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BACKLOG 20

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
  hints.ai_flags = AI_PASSIVE;

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

  if (bind(sockid, server->ai_addr, server->ai_addrlen) == -1) {
    perror("bind(): ");
    exit(4);
  }

  freeaddrinfo(server);

  if (listen(sockid, BACKLOG) == -1) {
    perror("listen(): ");
    exit(5);
  }

  int client;
  struct sockaddr_storage client_addr;
  char client_ip_string[30];
  socklen_t addrlen = sizeof client_addr;

  while (1) {
    client = accept(sockid, (struct sockaddr *)&client_addr, &addrlen);
    if (client == -1) {
      perror("accept(): ");
      exit(6);
    }
    // TODO Better way to give addr
    inet_ntop(client_addr.ss_family,
              &((struct sockaddr_in *)&client_addr)->sin_addr, client_ip_string,
              sizeof client_ip_string);
    printf("Connection : %s\n", client_ip_string);
    sleep(2);
    close(client);
  }
  close(sockid);
  return 0;
}
