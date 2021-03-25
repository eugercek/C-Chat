#include "netutils.h"
#include "shared.h"

// Network
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

// UNIX
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sysexits.h> // For Meaningful return values
#include <unistd.h>

// stdlib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BACKLOG 20
#define BUFFER_SIZE 256
#define THREAD_NUMBER 12
#define MAX_USERNAME_SIZE 16

static int thread_i = 0; // thread_i also says how many client have created.

static int clients[THREAD_NUMBER];

void *client_handler(void *arg) {
  printf("%lu come\n", pthread_self());
  char buffer[BUFFER_SIZE];
  int sockfd = ((struct client_t *)arg)->sockfd;
  int ret;
  free(arg);

  const char *welcome = "Welcome !\nEnter a username:";
  if (send_all(sockfd, welcome, strlen(welcome)) == -1)
    error_exit("send_all", EX_UNAVAILABLE);

  char username[MAX_USERNAME_SIZE];
  if (recv(sockfd, username, MAX_USERNAME_SIZE, 0) < 0)
    error_exit("recv", EX_UNAVAILABLE);

  printf("%s joined!\n", username);
  snprintf(buffer, BUFFER_SIZE, "%s joined!\n", username);
  for (int i = 0; i < thread_i; i++) {
    if (sockfd == clients[i])
      continue;
    if (send_all(clients[i], buffer, BUFFER_SIZE) == -1)
      error_exit("send_all", EX_UNAVAILABLE);
  }
  fflush(stdout);

  // TODO Check is antoher user has already connected with same username ?

  while (1) {
    ret = recv(sockfd, buffer, BUFFER_SIZE, 0);
    if (ret < 0)
      error_exit("recv", EX_UNAVAILABLE);
    else if (ret == 0) // Client closed connection
      break;

    printf("%s\t:%s\n", username, buffer);
    char bigger_buffer[BUFFER_SIZE + 100];
    char *time_str = hour_minute();
    snprintf(bigger_buffer, BUFFER_SIZE + 100, "%s\t:%-48s%s\n", username,
             buffer, time_str);
    free(time_str);
    for (int i = 0; i < thread_i; i++) {
      if (sockfd == clients[i])
        continue;
      if (send_all(clients[i], bigger_buffer, BUFFER_SIZE + 100) == -1)
        error_exit("send_all", EX_UNAVAILABLE);
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  struct addrinfo hints, *server;
  int sockfd;
  if (argc != 2) {
    fprintf(stderr, "Usage : %s port_no\n", argv[0]);
    exit(EX_USAGE);
  }

  // TODO Add proper signal handling
  // If client stopped/killed however and socket is not closed
  // And server tries do something with that socket
  // It'll get SIGPIPE and program will get killed
  struct sigaction sa;
  sa.sa_handler = SIG_IGN;
  sigaction(SIGPIPE, &sa, 0);

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

  // To use same port without waiting TIME_WAIT
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    error_exit("setsockopt", EX_UNAVAILABLE);

  if (bind(sockfd, server->ai_addr, server->ai_addrlen) == -1)
    error_exit("bind", EX_UNAVAILABLE);

  freeaddrinfo(server);

  if (listen(sockfd, BACKLOG) == -1)
    error_exit("listen", EX_UNAVAILABLE);

  int clientfd;
  struct sockaddr_storage client_addr;
  char client_ip_string[INET6_ADDRSTRLEN];
  socklen_t addrlen = sizeof client_addr;

  pthread_t tid[THREAD_NUMBER];

  while (1) { // Accepting new clients to server
    clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen);

    if (clientfd == -1)
      error_exit("accept", EX_UNAVAILABLE);

    // Need Dynamic memory or bad things happen
    struct client_t *p_client = malloc(sizeof(struct client_t));

    if (p_client == NULL)
      error_exit("malloc", EX_UNAVAILABLE);

    p_client->sockfd = clientfd;

    // TODO Better way to give addr
    inet_ntop(client_addr.ss_family,
              &((struct sockaddr_in *)&client_addr)->sin_addr, client_ip_string,
              sizeof client_ip_string);

    printf("Connection from : %s\n", client_ip_string);

    if (pthread_create(tid + thread_i, NULL, client_handler,
                       (void *)p_client) != 0)
      error_exit("pthread_create", EX_UNAVAILABLE);
    clients[thread_i] = clientfd;

    pthread_detach(tid[thread_i]);
    thread_i++;
  }
  close(sockfd);
  return 0;
}
