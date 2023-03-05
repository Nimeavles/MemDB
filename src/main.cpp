#include <iostream>
#include "server.h"

int main() {
  Server server;
  server.create_socket();
  server.avoid_ports_collision();
  server.sock_listen(3000);
  server.new_connection();

}