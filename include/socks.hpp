#ifndef SOCKS_HPP
#define SOCKS_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

struct sockaddr_in;

namespace sock {
    
    void create(int& s);

    void configure(sockaddr_in& addr, const int& port);

    void configure(sockaddr_in& addr, const int& port, const char* ip);

    void bind(int& s, sockaddr_in& addr);

    void listen(int& s, const int& max, const int& port);

    void connect(int& s, sockaddr_in& addr);

    void send(const int& s, const std::string& message);

    // for client side.
    void send_order(const int& s, const char* order);

    // for client side.
    void receive(const int& s, const int& buffer_size);

}

#endif // SOCKS_HPP