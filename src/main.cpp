#include "server.h"

int main() {
  Server server;
  server.setup_server();
  server.handle_connections(); 
}