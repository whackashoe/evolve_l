#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <vector>
#include <iostream>
#include "levenshtein_distance.hpp"
#include "lrule.hpp"
#include "rng.hpp"

struct LSystem
{
    std::vector<LRule> rules;
    std::vector<int> data;

    LSystem();

    bool match(const int data_p, const int rule_p) const;

    void iterate();

    void iterate(const std::vector<int> & axiom, const int n);

    int distance(const std::vector<int> & target) const;

    void mutate(Rng & rng);

    static LSystem generate_random_lsystem(Rng & rng);

    static LSystem sexual_reproduction(const LSystem & a, const LSystem & b, Rng & rng);
};

#endif