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

    //default settings here
    Settings() : 
    input(),
    target(),
    population_size(32),    //amount of concurrent systems per generation
    grammar_size(8),        //variety of grammar to generate (3 would be A B C)
    run_iterations(10),     //how many iterations should the system run during rewrite stage
    max_iterations(100000), //how many total iterations should we do
    print_regularity(10)    //how many iterations between printing current status
    {}

    void init_train(const size_t train_length);

    void print_input_and_target();
};

#endif