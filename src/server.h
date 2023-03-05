#pragma once
#include <string>
#include <netinet/in.h>
#define MAX_CONNECTIONS 10

class Server {
    public:
        int port = 3000;
        void create_socket();
        void avoid_ports_collision();
        void sock_listen(int port);
        void sock_listen();
        void new_connection();
    private:
        int _socket_fd;
        int opt = 1;
        struct sockaddr_in address;
        void bind_port(int port);
        int _actual_socket;
        int addrlen = sizeof(address);
};