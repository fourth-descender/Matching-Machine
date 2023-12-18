#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <queue>
#include <ctime>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <optional>

namespace types {
    class order {
        public:

            enum class side {
                buy,
                sell
            };

            order() = default;

            order(const side& side, const std::string& id, const std::string& symbol, const double& price, const int& quantity, const std::time_t& timestamp) :
                m_side(side),
                m_id(id),
                m_symbol(symbol),
                m_price(price),
                m_quantity(quantity),
                m_timestamp(timestamp) {
            };

            side get_side() const {
                return m_side;
            };

            std::string get_id() const {
                return m_id;
            };

            std::string get_symbol() const {
                return m_symbol;
            };

            double get_price() const {
                return m_price;
            };

            int get_quantity() const {
                return m_quantity;
            };

            bool operator>(const order& rhs) const {
                if (m_price == rhs.m_price) {
                    return m_timestamp > rhs.m_timestamp;
                }
                return m_price > rhs.m_price;
            };

            friend std::ostream& operator<<(std::ostream& os, const order& order) {
                std::string term = (bool) order.get_side() ? "seller " : "buyer ";
                os << term << order.get_id();
                return os;
            };

        private:
            side m_side;
            std::string m_id;
            std::string m_symbol;
            double m_price;
            int m_quantity;
            std::time_t m_timestamp;
    };

    class p_queue {
        public:
            p_queue() = default;
            
            void push(const order& order) {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_queue.push(order);
            };

            std::optional<order> pop() {
                std::lock_guard<std::mutex> lock(m_mutex);
                if (m_queue.empty()) {
                    return std::nullopt;
                }
                auto top = m_queue.top();
                m_queue.pop();
                return top;
            };

            std::optional<order> top() {
                std::lock_guard<std::mutex> lock(m_mutex);
                if (m_queue.empty()) {
                    return std::nullopt;
                }
                return m_queue.top();
            };

            bool empty() {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_queue.empty();
            };

            size_t size() {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_queue.size();
            };

        private:
            std::priority_queue<order, std::vector<order>, std::greater<order>> m_queue;
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
