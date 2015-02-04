#ifndef LRULE_H
#define LRULE_H

#include <vector>
#include <random>

struct LRule
{
    std::vector<int> from;
    std::vector<int> to;

    LRule(
        std::mt19937_64 & gen,
        std::uniform_int_distribution<int> & rng_dis, 
        std::uniform_int_distribution<int> & grammar_dis
    ) : from(), to() {
        do {
            from.push_back(grammar_dis(gen));
        } while(rng_dis(gen) < 200);

        do {
            to.push_back(grammar_dis(gen));
        } while(rng_dis(gen) < 200);
    }

    LRule(std::vector<int> from, std::vector<int> to) : from(from), to(to) {}
};

#endif