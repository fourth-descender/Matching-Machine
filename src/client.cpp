#include "jobs.hpp"
#include "socks.hpp"
#include "gen.hpp"

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define NUM_ORDERS 100000
#define MAX_THREADS 5
#define MAX_CLIENTS 5

int main() {
    job::queue jobs{MAX_THREADS};
    int client;
    sock::create(client);
    sockaddr_in addr;
    sock::configure(addr, SERVER_PORT, SERVER_ADDRESS);
    sock::connect(client, addr);

    std::thread receiver(sock::receive, client, BUFFER_SIZE);

    for (int i = 0; i < NUM_ORDERS; ++i) {
        jobs.enqueue(sock::send_order, client, gen::generate_order(i));
    };

    receiver.join();
    close(client);
    return 0;
};
