#ifndef SERVER_HPP
#define SERVER_HPP

#include <atomic>
#include <string>
#include <chrono>
#include <fcntl.h>
#include "jobs.hpp"
#include "socks.hpp"
#include "engine.hpp"

#define MAX_THREADS 5
#define PORT 12345
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 5

class server {
    public:
        server();
        ~server();
        void run();
    private:
        void handle(int client);
        void process(std::shared_ptr<std::istringstream> iss);
        bool parse(std::string& line, types::order& order);

        std::mutex m_mutex;
        matcher::engine m_engine;
        job::queue m_queue{MAX_THREADS};
        int m_socket, m_client;
        std::atomic <bool> m_running{true};
        sockaddr_in m_addr, m_client_addr;
        socklen_t m_client_len;
};

#endif // SERVER_HPP