#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <cassert>
#include "lsystem.hpp"
#include "rng.hpp"
#include "settings.hpp"


struct Universe
{
    std::vector<LSystem> population;

    Universe(Settings & settings) : population {}
    {
        assert("you probably don't intend for your input to be totally empty, try calling: `settings.init_train()` to populate with random data" && settings.input.size() != 0);

        populate_universe(settings);
    }

    void populate_universe(Settings & settings);

    void run(Settings & settings);
};

#endif