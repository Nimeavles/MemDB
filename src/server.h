#pragma once
#include <string>
#include <netinet/in.h>
#include <sys/epoll.h>

#define MAX_CONNECTIONS 4
#define PORT 8080
#define OPT 1

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
        struct sockaddr_in _client_address;
        int _client;
        socklen_t _socklen = sizeof(sockaddr);
        char _client_ip[INET_ADDRSTRLEN];
        uint16_t _client_port;
        void _get_client_info(int fd);

        //Epoll
        int _epoll_fd;
        struct epoll_event _event, _events[MAX_CONNECTIONS];
        void _delete_connection(int fd);

        //Read from client
        void _read_from_client(int fd, size_t bytes_to_read);
};
