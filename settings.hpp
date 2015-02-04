#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include "rng.hpp"

struct Settings
{
    std::vector<int> input;
    std::vector<int> target;

    size_t population_size;
    size_t grammar_size;
    size_t run_iterations;
    size_t max_iterations;

    Settings() : 
    input(),
    target(),
    population_size(30), 
    grammar_size(8), 
    run_iterations(10), 
    max_iterations(100000) {}

    void init_train(Rng & rng, const size_t train_length)
    {
        for(size_t i=0; i<train_length; ++i) {
            input.push_back(rng.grammar_dis(rng.gen));
            target.push_back(rng.grammar_dis(rng.gen));
        }
    }

    void print_input_and_target()
    {
        std::cout << "INPUT: "  << std::endl << "\t";
        for(size_t i=0; i<input.size(); ++i) {
            std::cout << input[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "TARGET: " << std::endl << "\t";
        for(size_t i=0; i<target.size(); ++i) {
            std::cout << target[i] << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
};

#endif