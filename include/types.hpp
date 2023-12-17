#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <queue>
#include <ctime>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <vector>

namespace types {
    class order {
        public:

            enum class side {
                BUY,
                SELL
            };

            order() = default;

            bool operator>(const order& rhs) const {
                if (m_price == rhs.m_price) {
                    return m_timestamp > rhs.m_timestamp;
                }
                return m_price > rhs.m_price;
            }

        private:
            std::string m_id;
            std::string m_symbol;
            side m_side;
            double m_price;
            double m_quantity;
            std::time_t m_timestamp;
    };

    class p_queue {
        public:
            p_queue() = default;
            
            void push(const order& order) {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_queue.push(order);
            }

            void pop() {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_queue.pop();
            }

            order top() {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_queue.top();
            }

            bool empty() {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_queue.empty();
            }

            size_t size() {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_queue.size();
            }

        private:
            std::priority_queue<order, std::vector<order>, std::greater<order>()> m_queue;
            std::mutex m_mutex;
    };

    class book {
        public:
            std::mutex m_mutex;
            std::unordered_set<std::string> m_symbols;
            std::unordered_map<std::string, p_queue> m_bids;
            std::unordered_map<std::string, p_queue> m_asks;
    };
}

#endif // TYPES_HPP
