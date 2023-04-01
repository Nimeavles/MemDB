#pragma once
#include <string>
#include <netinet/in.h>
#include <sys/epoll.h>
#include "client.h"

#define MAX_CONNECTIONS 4
#define PORT 8080

class Server {
    public:
        int port = PORT;
        void setup_server();
        void handle_connections();
    private:
        //Server
        int _server_fd;
        struct sockaddr_in _server_address;

        //Client
        int _client;
        socklen_t _socklen = sizeof(sockaddr);
        struct ClientInfo _client_info;

        //Epoll
        int _epoll_fd;
        struct epoll_event _event, _events[MAX_CONNECTIONS];
        void _delete_connection(int fd);

        //Read from client
        void _read_from_client(int fd, size_t bytes_to_read);
};
