#ifndef __NETUTILS_H_
#define __NETUTILS_H_

#include <stddef.h>

// Learned from Beej's guide to network
// send() doesn't send full data always
// It'll return number of byte is sent

int send_all(const int sockfd, const char *buffer, const int len);

#endif // __NETUTILS_H_
