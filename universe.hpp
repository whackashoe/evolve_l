#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <cassert>
#include "lsystem.hpp"
#include "rng.hpp"
#include "settings.hpp"


struct Universe
{
    Settings settings;
    Rng rng;
    std::vector<LSystem> population;

    Universe(const Settings & settings) : settings(settings), rng{ settings.grammar_size }, population {}
    {
        assert("you probably don't intend for your input to be totally empty, try calling: `settings.init_train()` to populate with random data" && settings.input.size() != 0);

        populate_universe();
    }

    void populate_universe();

    void run();
};

#endif