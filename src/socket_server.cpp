#include <sys/socket.h> // socket()
#include <cstdio> // perror()
#include <unistd.h> // exit()
#include "server.h"



using namespace std;


/*
  --CREATE SOCKET STEP--

  AF_INET defines that our program will run with IPV4. Providing that you want to use IPV6,
  you ought to use AF_INET6.

  SOCK_STREAM defines that the protocol that is going to be used is TCP. In case, you want to use UDP, just change it for SOCK_DGRAM.

  The int value passed as the last parameter, means the protocol to be used. In this case,
  0 value is assigned to the Internet Protocol (IP). 
*/

void Server::create_socket() {
  this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->_socket_fd <= 0) {
    perror("Could not create socket");
    exit(1);
  }
}

void Server::avoid_ports_collision() {
  if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->opt,sizeof(this->opt))) {
    perror("The port is taken by another program");
    exit(1);
  }
}

/*
  --BIND THE SOCKET TO A PORT--

  In order to run the socket server on a determinated port, we attach the server to the provided port,
  either the one you pass as parameter when calling the function `bind_port` or just the defaulut one
  `3000`.
*/

void Server::bind_port(int port) {
  this->address.sin_family = AF_INET;
  this->address.sin_addr.s_addr = INADDR_ANY;
  this->address.sin_port = htons(port);

  if (bind(this->_socket_fd, (struct sockaddr*)&this->address, sizeof(this->address)) < 0) {
    perror("Could not bind the socket to the port");
    exit(1);
  }
}

/*
  --LISTEN STEP--

  It puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection. The backlog, defines the maximum length to which the queue of pending connections for sockfd may grow. If a connection request arrives when the queue is full, the client may receive an error with an indication of `ECONNREFUSED`.
*/

void Server::sock_listen(int port) {
  this->port = port;

  this->bind_port(this->port);

  if (listen(this->_socket_fd, MAX_CONNECTIONS) < 0) {
    perror("Could not set the socket in a listen mode");
    exit(1);
  }
}

void Server::sock_listen() {
  this->bind_port(this->port);

  if (listen(this->_socket_fd, MAX_CONNECTIONS) < 0) {
    perror("Could not set the socket in a listen mode");
    exit(1);
  }
}

void Server::new_connection() {
  if ((this->_actual_socket = accept(this->_socket_fd, (struct sockaddr*)&address, (socklen_t*)&this->addrlen)) < 0) {
      perror("Could not accept new connection");
      exit(1);
    }
}
