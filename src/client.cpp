#include <arpa/inet.h>
#include "client.h"

ClientInfo ClientInfo::from_fd(int fd) {
  ClientInfo info;
  struct sockaddr_in addr;
  socklen_t len = sizeof(sockaddr);

  getpeername(fd, (struct sockaddr *)&addr, &len);
  inet_ntop(AF_INET, &addr.sin_addr, info.ip, INET_ADDRSTRLEN);

  info.port = ntohs(addr.sin_port);
  return info;
}
