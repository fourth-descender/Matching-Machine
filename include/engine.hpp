#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <sstream>
#include <mutex>
#include <optional>
#include "jobs.hpp"
#include "types.hpp"
#include "socks.hpp"

#define MAX_THREADS 5
namespace matcher {
    class engine {
        public:
            engine();
            ~engine() = default;
            void add_client(const int& client);
            void remove_client(const int& client);
            void process(const types::order& order);
        private:
            void match();
            void send(const std::string& message);
            void insert(const std::string& symbol);
            void enqueue(const types::order& order);
            void trade(const types::order& bid, const types::order& ask, const double& amount);
            void process_symbol(const std::string& symbol);
            std::unique_ptr<types::book> m_book;
            job::queue m_queue{MAX_THREADS};
            std::unordered_set<int> m_clients;

            std::mutex m_mutexes_mutex;
            std::unordered_map<std::string, std::shared_ptr<std::mutex>> m_mutexes;
    };
}

#endif // ENGINE_HPP