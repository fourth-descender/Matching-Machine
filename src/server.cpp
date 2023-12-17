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
    sock::bind(m_socket, m_addr);
    sock::listen(m_socket, MAX_CLIENTS, PORT);
};

server::~server() {
    m_running = false;
    close(m_socket);
};

void server::handle() {
    while (m_running) {
        char buffer[BUFFER_SIZE];
        ssize_t bytes = recv(m_client, buffer, BUFFER_SIZE - 1, 0);

        if (bytes < 0) {
            std::cerr << "Error reading from socket.\n";
            exit(EXIT_FAILURE);
        };

        if (bytes == 0) {
            std::cout << "Client disconnected.\n";
            break;
        };

        buffer[bytes] = '\0';
        std::shared_ptr<std::istringstream> iss = std::make_shared<std::istringstream>(buffer);
        m_queue.enqueue([this, iss] {
            process(iss);
        });
    };

    close(m_client);
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
        if (m_client = accept(m_socket, reinterpret_cast<struct sockaddr*>(&m_client_addr), &m_client_len); m_client < 0) {
            std::cerr << "Error accepting client.\n";
            exit(EXIT_FAILURE);
        };

        m_engine.set_client(m_client);
        m_queue.enqueue([this] {
            handle();
        });
    };
};