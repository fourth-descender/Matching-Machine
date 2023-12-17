#include "engine.hpp"

namespace matcher {
    engine::engine() :
        m_client(-1) {
    };

    void engine::set_client(const int& client) {
        m_client = client;
    };

    int engine::get_client() {
        return m_client;
    };

    void engine::insert(const std::string& symbol) {
        std::lock_guard<std::mutex> lock(m_book->m_mutex);
        if (m_book->m_symbols.find(symbol) == m_book->m_symbols.end()) {
            m_book->m_symbols.insert(symbol);
        };

        std::lock_guard<std::mutex> lock_for_mutexes(m_mutexes_mutex);
        if (m_mutexes.find(symbol) == m_mutexes.end()) {
            m_mutexes.emplace(symbol, std::make_shared<std::mutex>());
        };
    };

    void engine::enqueue(const types::order& order) {
        std::lock_guard<std::mutex> lock(m_book->m_mutex);
        if (order.get_side() == types::order::side::buy) {
            m_book->m_bids[order.get_symbol()].push(order);
            return;
        };
        m_book->m_asks[order.get_symbol()].push(order);
    };

    void engine::send(const std::string& message) {
        if (m_client > 0) {
            m_queue.enqueue([this, message] {
                sock::send(m_client, message);
            });
        };
    };

    void engine::trade(const types::order& bid, const types::order& ask, const double& amount) {
        std::lock_guard<std::mutex> lock(*m_mutexes[bid.get_symbol()]);
        std::ostringstream oss;
        oss << "matched: " << bid << " with " << ask 
            << " for " << amount << " shares of " << bid.get_symbol() 
            << " for $" << bid.get_price() << ".\n";
            
        send(oss.str());

        m_book->m_bids[bid.get_symbol()].pop();
        m_book->m_asks[ask.get_symbol()].pop();
    };

    void engine::process_symbol(const std::string& symbol) {
        std::lock_guard<std::mutex> lock(*m_mutexes[symbol]);
        auto& bids = m_book->m_bids[symbol];
        auto& asks = m_book->m_asks[symbol];
        while (!bids.empty() && !asks.empty()) {
            const auto bid_top = bids.top();
            const auto ask_top = asks.top();
            if (!bid_top.has_value() || !ask_top.has_value()) {
                return;
            };

            const auto bid = bid_top.value();
            const auto ask = ask_top.value();

            if (bid.get_price() < ask.get_price()) {
                return;
            };

            const auto& amount = std::min(bid.get_quantity(), ask.get_quantity());
            trade(bid, ask, amount);

            if (bid.get_quantity() < ask.get_quantity()) {
                const auto& new_ask = types::order(
                    ask.get_side(), 
                    ask.get_id(), 
                    ask.get_symbol(), 
                    ask.get_price(), 
                    ask.get_quantity() - amount, 
                    std::time(nullptr)
                );
                enqueue(new_ask);
                continue;
            };

            if (bid.get_quantity() > ask.get_quantity()) {
                const auto& new_bid = types::order(
                    bid.get_side(), 
                    bid.get_id(), 
                    bid.get_symbol(), 
                    bid.get_price(), 
                    bid.get_quantity() - amount, 
                    std::time(nullptr)
                );
                enqueue(new_bid);
                continue;
            };

            return;
        };
    };

    void engine::process(const types::order& order) {
        if (!m_book) {
            m_book = std::make_unique<types::book>();
        };

        insert(order.get_symbol());
        enqueue(order);
        match();
    };

    void engine::match() {
        for (const auto& symbol : m_book->m_symbols) {
            m_queue.enqueue([this, symbol]() {
                process_symbol(symbol);
            });
        };
    }
}