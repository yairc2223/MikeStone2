//
// Created by yair on 14/1/20.
//

#ifndef MILESTONE2_SERVER_H
#define MILESTONE2_SERVER_H

#include <iostream>
#include "ClientHandler.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <string>
#include <system_error>
#include <stdexcept>
using namespace std;
namespace server_side {
    class Server {
    public:
        virtual void openServer(int port, ClientHandler* clientHandler) = 0;
        virtual void stopServer() = 0;
        virtual bool isOpen() = 0;
    };
}



namespace posix_sockets {
    class timeout_exception : public  ::runtime_error {
    public:
        explicit timeout_exception(const char *msg) :  ::runtime_error(msg) {}

        explicit timeout_exception(const  ::string& msg) :  runtime_error(msg) {}
    };

    struct TCP_socket {
        int sock_fd;

        TCP_socket() {
            sock_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (sock_fd < 0) {
                throw  system_error( error_code(errno,  generic_category()), "couldn't open socket");
            }
        }

        TCP_socket(int open_sock_fd) {
            this->sock_fd = open_sock_fd;
        }

        void close() {
            if (::close(sock_fd) < 0) {
                throw system_error( error_code(errno,  generic_category()), "couldn't close  socket");
            }
        }

        void settimeout(int sec, int usec = 0) {
            timeval timeout;
            timeout.tv_sec = sec;
            timeout.tv_usec = usec;
            if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO,
                           (char *) &timeout, sizeof(timeout)) == -1) {
                throw  system_error( error_code(errno,  generic_category()), "couldn't set socket");
            }
        }
    };

    class TCP_client {
        TCP_socket sockett;
    public:
        int get_socket() const {
            return sockett.sock_fd;
        }
        explicit TCP_client(TCP_socket sock) : sockett(sock) {}
         string read(int max_length) {
            char *buffer = new char[max_length + 1];
            int read_length = ::read(sockett.sock_fd, buffer, max_length);
            if (read_length < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    throw timeout_exception("timeout on read");
                }
                throw  system_error( error_code(errno,  generic_category()), "can't read");
            }

            buffer[read_length] = 0;
             string output =  string(buffer);
            delete[] buffer;
            return output;
        }

        void settimeout(int sec, int usec = 0) {
            sockett.settimeout(sec, usec);
        }

        void close() {
            sockett.close();
        }
    };

    class TCP_server {
        TCP_socket sockserver;
    public:
        TCP_server(int port) {
            int n = 1;
            setsockopt(sockserver.sock_fd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(int));
            sockaddr_in addr_in;
            addr_in.sin_family = AF_INET;
            addr_in.sin_port = htons(port);
            addr_in.sin_addr.s_addr = INADDR_ANY;

            if (bind(sockserver.sock_fd, (sockaddr *) &addr_in, sizeof(addr_in)) == -1) {
                throw  system_error( error_code(errno,  generic_category()), "couldn't bind socket");
            }
        }

        void listen(int max_lis) {
            if (::listen(sockserver.sock_fd, max_lis) == -1) {
                throw  system_error( error_code(errno,  generic_category()), "couldn't listen");
            }
        }

        void settimeout(int sec, int usec = 0) {
            sockserver.settimeout(sec, usec);
        }

        TCP_client accept() {
            sockaddr_in addr;
            socklen_t length = sizeof(addr);
            int client_sock_fd = ::accept(sockserver.sock_fd, (sockaddr*) &addr, &length);
            if (client_sock_fd < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    throw timeout_exception("TIMEOUT");
                } else {
                    throw  system_error( error_code(errno,  generic_category()), "couldn't accept");
                }
            }

            TCP_socket client_sock(client_sock_fd);
            client_sock.settimeout(0);
            return TCP_client(client_sock);
        }

        void close() {
            sockserver.close();
        }
    };
}
#endif //MILESTONE2_SERVER_H
