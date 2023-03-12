#include <sys/socket.h> // socket()
#include <cstdio> // perror()
#include <unistd.h> // exit()
#include <cstring>
#include "server.h"

using namespace std;

char *message = "Hello World\n";

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
  if (this->_socket_fd < 0) {
    perror("Could not create the socket");
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
	this->server_address.sin_family = AF_INET;
	this->server_address.sin_addr.s_addr = INADDR_ANY;
	this->server_address.sin_port = htons(port);
    bzero(&(this->server_address.sin_zero), 8);

	if (bind(this->_socket_fd, (struct sockaddr*)&this->server_address, this->addrlen) == -1) {
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

	if (listen(this->_socket_fd, MAX_CONNECTIONS) == -1) {
		perror("Could not set the socket in a listen mode");
		exit(1);
	}
}

void Server::sock_listen() {
	this->bind_port(this->port);

	if (listen(this->_socket_fd, MAX_CONNECTIONS) == -1) {
	    perror("Could not set the socket in a listen mode");
	    exit(1);
	}
}

void Server::new_connection() {
    
    while(1) {
        size_t client_length = sizeof(struct sockaddr_in);

        if ((this->_actual_socket = accept(this->_socket_fd, (struct sockaddr*)&this->client_address, (socklen_t*)&client_length)) < 0) {
		    perror("Could not accept new connection");
		    exit(1);
	    }
        
        int valread = read(this->_actual_socket, this->_buffer, 1024);
        puts(this->_buffer);	
	    send(this->_actual_socket, message, strlen(message), 0);
	    puts("Hello message sent\n");
        
    }
    close(this->_actual_socket);
    close(this->_socket_fd);
}
