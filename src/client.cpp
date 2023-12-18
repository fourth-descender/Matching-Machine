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
        jobs.enqueue(sock::send_order, client, gen::generate_order(i + 1));
    };

    // std::shared_ptr<char[]> shared_orders[] = {
    //     // time-priority -> matches buyer 1 and seller 2.
    //     std::shared_ptr<char[]>(strdup("SELL 2 ABC 100 2\n"), free),
    //     std::shared_ptr<char[]>(strdup("SELL 3 ABC 100 2\n"), free),
    //     std::shared_ptr<char[]>(strdup("BUY 1 ABC 100 2\n"), free),
    //     // best price -> matches buyer 11 and seller 13.
    //     std::shared_ptr<char[]>(strdup("SELL 13 BAC 99 1\n"), free),
    //     std::shared_ptr<char[]>(strdup("SELL 12 BAC 100 1\n"), free),
    //     std::shared_ptr<char[]>(strdup("BUY 11 BAC 100 1\n"), free),
    //     // partial fill -> matches buyer 21 with both seller 22 and 23.
    //     std::shared_ptr<char[]>(strdup("SELL 23 CAB 99 1\n"), free),
    //     std::shared_ptr<char[]>(strdup("SELL 22 CAB 100 2\n"), free),
    //     std::shared_ptr<char[]>(strdup("BUY 21 CAB 100 4\n"), free)
    // };

    // for (int i = 0; i < 9; i++) {
    //     jobs.enqueue(sock::send_order, client, shared_orders[i]);
    // };

    receiver.join();
    close(client);
    return 0;
};
