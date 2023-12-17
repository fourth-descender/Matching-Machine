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
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
    };

    void configure(sockaddr_in& addr, const int& port, const char* ip) {
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip);
        addr.sin_port = htons(port);
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
    };

    void connect(int& s, sockaddr_in& addr) {
        if (::connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Error connecting to server.\n";
            exit(EXIT_FAILURE);
        }
    };

    void send(int& s, const std::string& message) {
        if (::send(s, message.c_str(), message.size(), 0) < 0) {
            std::cerr << "Error sending message.\n";
            exit(EXIT_FAILURE);
        }
    };

    void send_order(int& s, const char* order) {
        if (::send(s, order, strlen(order), 0) < 0) {
            std::cerr << "Error sending order.\n";
            close(s);
            exit(EXIT_FAILURE);
        }

        // order is a pointer to a char array on the stack.
        delete[] order;
    };

    void receive(int& s, const int& buffer_size) {
        char buffer[buffer_size];
        ssize_t bytes_read;

        while (true) {
            bytes_read = recv(s, buffer, buffer_size, 0);
            if (bytes_read <= 0) {
                if (bytes_read == 0 || errno == ECONNRESET) {
                    std::cout << "Server disconnected. Exiting...\n";
                    break;
                };
                perror("recv failed!\n");
                break;
            };

            // null terminate the buffer.
            buffer[bytes_read] = '\0';
            std::cout << "Received from server: " << buffer << std::endl;
        };
    }
}
