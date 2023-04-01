#pragma once
#include <netinet/in.h>

struct ClientInfo {
  char ip[INET_ADDRSTRLEN];
  uint16_t port;

  static ClientInfo from_fd(int fd);
};