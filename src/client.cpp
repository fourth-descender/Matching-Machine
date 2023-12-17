#include "jobs.hpp"
#include "socks.hpp"
#include "gen.hpp"

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define NUM_ORDERS 1000000
#define MAX_THREADS 5

int main() {
    int client;
    sockaddr_in addr;
    job::queue jobs{MAX_THREADS};

    sock::create(client);
    sock::configure(addr, SERVER_PORT, SERVER_ADDRESS);
    sock::connect(client, addr);

    std::thread receiver(sock::receive, client, BUFFER_SIZE);

    for (int i = 0; i < NUM_ORDERS; i++) {
        jobs.enqueue(sock::send_order, client, gen::generate_order(i + 1));
    };

    // sample orders, remove delete[] order in send_order to use.
    // used for test cases -> passed all three cases.
    // const char* orders[] = {
    //    //  time priority.
    //     "SELL 2 ABC 100 2\n",
    //     "SELL 3 ABC 100 2\n",
    //     "BUY 1 ABC 100 2\n",

    //    //  best price.
    //     "SELL 13 BAC 99 1\n",
    //     "SELL 12 BAC 100 1\n",   
    //     "BUY 11 BAC 100 1\n",

    //    //  partial fills.
    //     "SELL 23 CAB 99 1\n",
    //     "SELL 22 CAB 100 2\n",
    //     "BUY 21 CAB 100 4\n",
    // };

    // for (int i = 0; i < 9; i++) {
    //     jobs.enqueue(sock::send_order, client, orders[i]);
    // };

    receiver.join();
    close(client);
    return 0;
};
