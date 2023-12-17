#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <sstream>
#include "jobs.hpp"
#include "types.hpp"

#define MAX_THREADS 5
namespace matcher {
    class engine {
        public:
            engine() = default;
            ~engine() = default;
            void process(const types::order& order);
        private:
            void match();
            void insert(const std::string& symbol);
            void enqueue(const types::order& order);
            void trade(const types::order& bid, const types::order& ask, const double& amount);
            void process_symbol(const std::string& symbol);
            std::unique_ptr<types::book> m_book;
            job::queue m_queue{MAX_THREADS};
    };
}

#endif // ENGINE_HPP