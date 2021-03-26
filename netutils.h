#ifndef __NETUTILS_H_
#define __NETUTILS_H_

#include <netinet/in.h>
#include <stddef.h>
#include <sys/socket.h>

// Learned from Beej's guide to network
// send() doesn't send full data always
// It'll return number of byte is sent

int send_all(const int sockfd, const char *buffer, const int len);

// Will implement protocol here.
struct client_t {
  int sockfd;
  char ip[INET6_ADDRSTRLEN];
};

// Beej' Guide to network again
void *get_in_addr(struct sockaddr *sa);

#endif // __NETUTILS_H_
