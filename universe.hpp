#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <cassert>
#include "lsystem.hpp"
#include "rng.hpp"
#include "settings.hpp"


struct Universe
{
    std::vector<int> input;
    std::vector<int> target;
    std::vector<LSystem> population;

    Universe(Settings & settings) : 
    input{},
    target{},
    population {}
    {
        populate_universe(settings);
    }

    void init_train(Settings & settings, const size_t train_length);
    void init_train(Settings & settings, const size_t input_length, const size_t target_length);

    void populate_universe(Settings & settings);

    void run(Settings & settings);

    void print_input_and_target();
};

#endif