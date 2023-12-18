#ifndef SOCKS_HPP
#define SOCKS_HPP

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <memory>
#include <netdb.h> 
#include <functional>

struct in_addr;
struct addrinfo;
struct sockaddr_in;

namespace sock {
    
    void create(int& s);

    void configure(sockaddr_in& addr, const int& port);

    void configure(sockaddr_in& addr, const int& port, const char* ip);

    void bind(int& s, sockaddr_in& addr);

    void listen(int& s, const int& max, const int& port);

    void connect(int& s, sockaddr_in& addr);

    void send(const int& s, const std::string& message);

    void receive(const int& s, std::string& received, const int& buffer_size);

    void process_received(std::string& received, std::function<void(std::string&)> func);

    // for client side.
    void send_order(const int& s, const std::shared_ptr<char[]>& order);

    // for client side.
    void receive_from_server(const int& s, const int& buffer_size);

    // for docker.
    in_addr resolve_hostname(const char* hostname);

}

#endif // SOCKS_HPP