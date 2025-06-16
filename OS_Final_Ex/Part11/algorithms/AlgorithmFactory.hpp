#ifndef ALGORITHM_FACTORY_HPP
#define ALGORITHM_FACTORY_HPP

#include "AlgorithmStrategy.hpp"
#include "MSTAlgorithm.hpp"
#include "SCCAlgorithm.hpp"
#include "CliqueAlgorithm.hpp"
#include "MaxFlowAlgorithm.hpp"
#include <memory>

using namespace std;

class AlgorithmFactory {
public:
    static unique_ptr<AlgorithmStrategy> create(int choice);
};

#endif
