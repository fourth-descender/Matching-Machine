#include "jobs.hpp"
#include "socks.hpp"
#include "gen.hpp"

#define SERVER_ADDRESS "127.0.0.1"
#ifdef DOCKER_ENV
    #define SERVER_ADDRESS "server"
#endif

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define NUM_ORDERS 1000000
#define MAX_THREADS 3

int main() {
    int client;
    sockaddr_in addr;
    job::queue jobs{MAX_THREADS};

    sock::create(client);
    sock::configure(addr, SERVER_PORT, SERVER_ADDRESS);
    sock::connect(client, addr);

    std::thread receiver(sock::receive_from_server, client, BUFFER_SIZE);

    for (int i = 0; i < NUM_ORDERS; i++) {
        jobs.enqueue(sock::send_order, client, gen::generate_order(NUM_ORDERS + i + 1));
    };

    receiver.join();
    close(client);
    return 0;
};
