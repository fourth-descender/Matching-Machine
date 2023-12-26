#include "server.hpp"

#define TIME(func) \
do { \
    auto start_time = std::chrono::high_resolution_clock::now(); \
    func; \
    auto end_time = std::chrono::high_resolution_clock::now(); \
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time); \
    std::cout << "Time taken to process order: " << duration.count() << " microseconds" << std::endl; \
} while(0)

server::server() {
    m_client_len = sizeof(struct sockaddr_in);
    sock::create(m_socket);
    sock::configure(m_addr, PORT);
    // set to non-blocking for multiple clients.
    fcntl(m_socket, F_SETFL, O_NONBLOCK);
    sock::bind(m_socket, m_addr);
    sock::listen(m_socket, MAX_CLIENTS, PORT);
};

server::~server() {
    m_running = false;
    close(m_socket);
};

void server::handle(int client) {
    std::string received; // Store the received data

    while (m_running) {
        bool success = sock::receive(client, received, BUFFER_SIZE, true);
        if (!success) {
            m_engine.remove_client(client);
            return;
        };
        sock::process_received(received, [this](std::string& message) {
            std::shared_ptr<std::istringstream> iss = std::make_shared<std::istringstream>(message);
            m_queue.enqueue([this, iss] {
                process(iss);
            });
        });
    };

    m_engine.remove_client(client);
    close(client);
};

bool server::parse(std::string& line, types::order& order) {
    std::istringstream iss(line);
    std::string side, id, symbol, price, quantity;
    iss >> side >> id >> symbol >> price >> quantity;
    if (iss.fail() || side != "BUY" && side != "SELL") {
        return false;
    };

    order = types::order(
        side == "BUY" ? types::order::side::buy : types::order::side::sell,
        id,
        symbol,
        std::stod(price),
        std::stod(quantity),
        std::time(nullptr)
    );

    return true;
};

void server::process(std::shared_ptr<std::istringstream> iss) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string line;
    while(std::getline(*iss, line, '\n')) {
        types::order order;
        if (!parse(line, order)) {
            std::cout << "Received: \n";
            std::cout << line << std::endl;
            std::cerr << "Error parsing order.\n";
            continue;
        };
        std::cout << "Received order: " << order << std::endl;
        TIME(m_engine.process(order));
    };
};

void server::run() {
    while (m_running) {
        int client;
        if ((client = accept(m_socket, reinterpret_cast<struct sockaddr*>(&m_client_addr), &m_client_len)) < 0) { 
            // std::cerr << "Error accepting client.\n";
            continue;
        };
        std::cout << "client connected: " << client << std::endl;
        m_engine.add_client(client);
        m_queue.enqueue([this, client] {
            handle(client);
        });
    };
};