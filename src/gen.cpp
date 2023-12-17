#include "gen.hpp"

const char* actions[] = {"BUY", "SELL"};
const char* symbols[] = {"ABC", "BAC", "CAB"};

namespace gen {
    
    int random_number(const int& min, const int& max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    };
    
    double normal_random(const double& mean, const double& stddev) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dis(mean, stddev);
        return dis(gen);
    };
    
    const char* generate_order(int id) {
        const char* action = actions[random_number(0, 1)];
        const char* symbol = symbols[random_number(0, 2)];
        double price = normal_random(97.5, 5);
        double quantity = random_number(25, 100);

        char* order = new char[100];

        // use snprintf to avoid buffer overflow as per warning instruction.
        snprintf(order, 100, "%s %d %d %d %s\n", action, id, price, quantity, symbol);

        return order;
    };

}