#ifndef SERVER_HPP
#define SERVER_HPP

#include <atomic>
#include <string>
#include "jobs.hpp"
#include "socks.hpp"
#include "engine.hpp"

#define MAX_THREADS 5
#define BUFFER_SIZE 1024

class server {
    public:
        server() = default;
        ~server() = default;
        void run();
    private:
        void handle();
        void sighandler(int signum);
        void process(std::shared_ptr<std::istringstream> iss);
        bool parse(std::string& line, types::order& order);

        std::mutex m_mutex;
        matcher::engine m_engine;
        job::queue m_queue{MAX_THREADS};
        int m_socket, m_client;
        std::atomic <bool> m_running{true};
        sockaddr_in m_addr, m_client_addr;
};

#endif // SERVER_HPP