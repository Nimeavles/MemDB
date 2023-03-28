#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CONNECTIONS 4
#define PORT 8080

void close_fd(int *epoll_fd, int *socket_fd) {
  if (close(*socket_fd) < 0) {
    fprintf(stderr, "Couldn't close the server fd\n");
    exit(EXIT_FAILURE);
  }

  if (close(*epoll_fd) < 0) {
    fprintf(stderr, "Couldn't close the epoll fd\n");
    exit(EXIT_FAILURE);
  }
}

int main() {
  //Set up socket and epoll FD
  int epoll_fd = epoll_create1(0);
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  //Epoll data
  struct epoll_event event, events[MAX_CONNECTIONS];
  
  //Socket data
  int opt = 1;
  struct sockaddr_in server_address;
  socklen_t socklen = sizeof(server_address);
  char buffer[1024] = { 0 };
  
  //Client info
  struct sockaddr_in client_address;
  char client_ip[INET_ADDRSTRLEN];
  uint16_t client_port;

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    fprintf(stderr, "Couldn't setsockopt\n");
    close_fd(&epoll_fd, &server_fd);
    return EXIT_FAILURE;
  }
  
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);
  bzero(&(server_address.sin_zero), 8);

  if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    fprintf(stderr, "Couldn't bind the socket to the port\n");
    close_fd(&epoll_fd, &server_fd);
    return EXIT_FAILURE;
  }

  if (listen(server_fd, MAX_CONNECTIONS) < 0) {
    fprintf(stderr, "Couldn't set the socket on listen mode\n");
    close_fd(&epoll_fd, &server_fd);
    return EXIT_FAILURE;
  }

  event.data.fd = server_fd;
  event.events = EPOLLIN;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) < 0) {
    fprintf(stderr, "Couldn't add the socket fd to the epoll\n");
    close_fd(&epoll_fd, &server_fd);
    return EXIT_FAILURE;
  }

  int RUNNING = 1;
  
  int client;

  while (RUNNING) {
    int event_count = epoll_wait(epoll_fd, events, MAX_CONNECTIONS, 3000);

    if (event_count == 0) {
      continue;
    }
    
    for(int i = 0; i < event_count; i++) {
      int fd = events[i].data.fd;
      if (fd == server_fd) {
        client = accept(server_fd, (struct sockaddr *)&server_address, &socklen);
        if (client < 0) {
          close(client);
          close_fd(&epoll_fd, &server_fd);
        }

        getpeername(client, (struct sockaddr *)&client_address, &socklen);
        inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
        client_port = ntohs(client_address.sin_port);

        printf("\033[32mNEW CONNECTION FROM %s:%d\033[0m\n", client_ip, client_port);

        event.data.fd = client;
        event.events = EPOLLIN;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &event) < 0) {
          fprintf(stderr, "\033[31mCouldn't add the new client fd to the epoll\033[0m\n");
          close(client);
          close_fd(&epoll_fd, &server_fd);
          return EXIT_FAILURE;
        }

        continue;
      }
        int valread = read(client, buffer, 1024);
        printf("\033[33m[%s:%d]\033[0m \033[34m=> %s\033[0m\n", client_ip, client_port, buffer);
        send(client, "Hello World\n",strlen("Hello World\n"), 1024);
    }
  }


  // Close all the FD  

  if (server_fd < 0) {
    fprintf(stderr, "Couldn't create socket fd\n");
    return EXIT_FAILURE;
  }

  if (epoll_fd < 0) {
    fprintf(stderr, "Couldn't create the epoll fd\n");
    return EXIT_FAILURE;
  }
}