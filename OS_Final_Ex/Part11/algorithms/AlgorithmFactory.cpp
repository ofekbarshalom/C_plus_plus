#include "AlgorithmFactory.hpp"

using namespace std;

unique_ptr<AlgorithmStrategy> AlgorithmFactory::create(int choice) {
    switch (choice) {
        case 1: return make_unique<EulerianAlgorithm>();
        case 2: return make_unique<MSTAlgorithm>();
        case 3: return make_unique<SCCAlgorithm>();
        case 4: return make_unique<CliqueAlgorithm>();
        case 5: return make_unique<MaxFlowAlgorithm>();
        default: return nullptr;
    }
}
