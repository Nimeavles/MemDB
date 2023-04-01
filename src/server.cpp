#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.h"

using namespace std;

string server_message = u8"Hello World from server\n";

void close_fd(int socket_fd) {
  if (close(socket_fd) < 0) {
    fprintf(stderr, "\033[31mCouldn't close the server fd\033[0m\n");
    exit(EXIT_FAILURE);
  }
}

void close_fd(int socket_fd, int epoll_fd) {
  if (close(socket_fd) < 0) {
    fprintf(stderr, "\033[31mCouldn't close the server fd\033[0m\n");
    exit(EXIT_FAILURE);
  }

  if (close(epoll_fd) < 0) {
    fprintf(stderr, "\033[31mCouldn't close the epoll fd\033[0m\n");
    exit(EXIT_FAILURE);
  }
}

void Server::_get_client_info(int fd) {
  getpeername(fd, (struct sockaddr *)&this->_client_address, &this->_socklen);
  inet_ntop(AF_INET, &this->_client_address.sin_addr, this->_client_ip, INET_ADDRSTRLEN);
  this->_client_port = ntohs(this->_client_address.sin_port);
}

void Server::_read_from_client(int fd, size_t bytes_to_read) {
  char buff[1024];

  ssize_t data_read = recv(fd, buff, bytes_to_read, 0);

  string buff_read(buff, data_read);

  if (data_read == 0) {
    cout << "\033[31m[" << this->_client_ip << ":" << this->_client_port << "] => " << "HAS CLOSED THE CONNETION\033[0m" << endl;
    this->_delete_connection(fd);
    close(fd);      
  }else if (data_read < 0) {
    cerr << "\033[31mCouldn't get data from the client\033[0m" << endl;
    close_fd(this->_server_fd, this->_epoll_fd); 
  }else {
    cout << "\033[33m[" << this->_client_ip << ":" << this->_client_port <<"]\033[0m\033[34m => " << buff_read << "\033[0m";
    send(fd, server_message.c_str() ,server_message.size(), 1024);
  }
}

void Server::_delete_connection(int fd) {
  epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, fd, NULL); 
}

void Server::setup_server() {
  static constexpr int opt = 1;

  this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) 
    || setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) ) {
    fprintf(stderr, "\033[31mCouldn't setsockopt\033[0m\n");
    close_fd(this->_server_fd);
  }

	this->_server_address.sin_family = AF_INET;
	this->_server_address.sin_addr.s_addr = INADDR_ANY;
	this->_server_address.sin_port = htons(PORT);

  if (bind(this->_server_fd, (struct sockaddr *)&this->_server_address, sizeof(this->_server_address)) < 0) {
    fprintf(stderr, "\033[31mCouldn't bind the socket to the port\033[0m\n");
    close_fd(this->_server_fd);
  }

  if (listen(this->_server_fd, MAX_CONNECTIONS) < 0) {
    fprintf(stderr, "\033[31mCouldn't set the socket on listen mode\033[0m\n");
    close_fd(this->_server_fd);
  }
}


void Server::handle_connections() {

  if (this->_server_fd < 0) {
    fprintf(stderr, "\033[31mPlease create a server before running this function\033[0m\n");
    exit(EXIT_FAILURE);
  }

  this->_epoll_fd = epoll_create1(0);

  this->_event.data.fd = this->_server_fd;
  this->_event.events = EPOLLIN;

  if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_server_fd, &this->_event) < 0) {
    fprintf(stderr, "\033[31mCouldn't add the socket fd to the epoll\033[0m\n");
    close_fd(this->_server_fd, this->_epoll_fd);
    exit(EXIT_FAILURE);
  }

  while(true) {
    int event_count = epoll_wait(this->_epoll_fd, this->_events, MAX_CONNECTIONS, 3000);

    if (event_count == 0) {
      continue;
    }

    for(int i = 0; i < event_count; i++) {
      int fd = this->_events[i].data.fd;

      if(fd == this->_server_fd) {
        this->_client = accept(this->_server_fd, (struct sockaddr *)&this->_server_address, &this->_socklen);

        if (this->_client < 0) {
          close(this->_client);
          continue;
        }
        
        this->_get_client_info(this->_client);
        printf("\033[32mNEW CONNECTION FROM %s:%d\033[0m\n", this->_client_ip, this->_client_port);
         
        this->_event.data.fd = this->_client;
        this->_event.events = EPOLLIN;

        if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_client, &this->_event) < 0) {
          fprintf(stderr, "\033[31mCouldn't add the new client fd to the epoll\033[0m\n");
          close(this->_client);
          close_fd(this->_server_fd, this->_epoll_fd);
          exit(EXIT_FAILURE);
        }
        continue;
      }else {
        this->_get_client_info(fd);
        this->_read_from_client(fd, 1024);
      }
    }    
  }
}