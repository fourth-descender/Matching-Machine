#include "socks.hpp"

namespace sock {
    void create(int& s) {
        s = ::socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0) {
            std::cerr << "Error creating socket.\n";
            exit(EXIT_FAILURE);
        }
    };

    void configure(sockaddr_in& addr, const int& port) {
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
    };

    void configure(sockaddr_in& addr, const int& port, const char* ip) {
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);
        // for docker -> 127.0.0.1 becomes the container itself.
        // could also be the case for a VM or invalid IP.
        if (addr.sin_addr.s_addr == INADDR_NONE) {
            addr.sin_addr = resolve_hostname(ip);
        }
    };

    void bind(int& s, sockaddr_in& addr) {
        if (::bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Error binding socket.\n";
            exit(EXIT_FAILURE);
        }
    };

    void listen(int& s, const int& max, const int& port) {
        if (::listen(s, max) < 0) {
            std::cerr << "Error listening on port " << port << ".\n";
            exit(EXIT_FAILURE);
        }
        std::cout << "Listening on port " << port << "...\n";
    };

    void connect(int& s, sockaddr_in& addr) {
        if (::connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Error connecting to server.\n";
            exit(EXIT_FAILURE);
        }
    };

    void send(const int& s, const std::string& message) {
        if (::send(s, message.c_str(), message.size(), 0) < 0) {
            std::cerr << "Error sending message.\n";
            exit(EXIT_FAILURE);
        }
    };

    void send_order(const int& s, const std::shared_ptr<char[]>& order) {
        if (::send(s, order.get(), strlen(order.get()), 0) < 0) {
            std::cerr << "Error sending order.\n";
            close(s);
            exit(EXIT_FAILURE);
        }
    };

    void receive(const int& s, std::string& received, const int& buffer_size) {
        char buffer[buffer_size];
        ssize_t bytes_read = recv(s, buffer, buffer_size - 1, 0);
        if (bytes_read < 0) {
            std::cerr << "Error reading from socket.\n";
            exit(EXIT_FAILURE);
        }

        if (bytes_read == 0) {
            std::cout << "Server disconnected.\n";
            close(s);
            exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';
        received.append(buffer);
    };

    void receive_from_server(const int& s, const int& buffer_size) {
        char buffer[buffer_size];
        ssize_t bytes_read;
        std::string received;

        while (true) {
            receive(s, received, buffer_size);
            process_received(received, [](std::string& message) {
                std::cout << message << std::endl;
            });
        }
    };

    void process_received(std::string& received, std::function<void(std::string&)> func) {
        // check if the received message is complete.
        size_t delimiter_pos;
        while ((delimiter_pos = received.find('\n')) != std::string::npos) {
            std::string message = received.substr(0, delimiter_pos);
            received.erase(0, delimiter_pos + 1);

            // process the complete message.
            func(message);
        }
    };

    in_addr resolve_hostname(const char* hostname) {
        addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
            std::cerr << "Error resolving hostname.\n";
            exit(EXIT_FAILURE);
        }

        in_addr addr = ((sockaddr_in*)res->ai_addr)->sin_addr;

        freeaddrinfo(res);

        return addr;
    };
}
