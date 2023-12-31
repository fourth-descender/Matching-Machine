#ifndef GEN_HPP
#define GEN_HPP

#include <memory>
#include <random>
#include <sstream>
#include "types.hpp"

namespace gen {

    int random_number(const int& min, const int& max);
    
    double normal_variable(const double& mean, const double& stddev);
    
    std::shared_ptr<char[]> generate_order(int order_id);

}

#endif // GEN_HPP