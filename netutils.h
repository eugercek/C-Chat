#ifndef __NETUTILS_H_
#define __NETUTILS_H_

#include <stddef.h>
#include <sys/socket.h>

// Learned from Beej's guide to network
// send() doesn't send full data always
// It'll return number of byte is sent

int send_all(const int sockfd, const char *buffer, const int len);

// Will implement protocol here.
struct client_t {
  int sockfd;
};

#endif // __NETUTILS_H_
