#include "settings.hpp"


void Settings::init_train(Rng & rng, const size_t train_length)
{
    for(size_t i=0; i<train_length; ++i) {
        input.push_back(rng.grammar_dis(rng.gen));
        target.push_back(rng.grammar_dis(rng.gen));
    }
}

void Settings::print_input_and_target()
{
    std::cout << "INPUT: "  << std::endl << "\t";
    for(size_t i=0; i<input.size(); ++i) {
        std::cout << (char) ('A' + input[i]) << " ";
    }
    std::cout << std::endl;

    std::cout << "TARGET: " << std::endl << "\t";
    for(size_t i=0; i<target.size(); ++i) {
        std::cout << (char) ('A' + target[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}