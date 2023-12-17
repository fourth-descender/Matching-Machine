#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <sstream>
#include "types.hpp"

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
    };
}

#endif // ENGINE_HPP