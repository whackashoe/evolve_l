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
    size_t print_regularity;

    Settings() : 
    input(),
    target(),
    population_size(30), 
    grammar_size(6), 
    run_iterations(10), 
    max_iterations(100000),
    print_regularity(10) {}

    void init_train(Rng & rng, const size_t train_length);

    void print_input_and_target();
};

#endif