#ifndef RNG_H
#define RNG_H

#include <random>

struct Rng
{
    std::random_device rd;
    std::mt19937_64 gen;
    std::uniform_int_distribution<int> grammar_dis;
    std::uniform_int_distribution<int> rng_dis;
    std::uniform_int_distribution<int> mutate_dis;
    std::uniform_int_distribution<int> mutate_ins_dis;
    std::uniform_int_distribution<int> mutate_rins_dis;

    Rng(const size_t grammar_size) : 
    rd(), gen(rd()), grammar_dis(0, grammar_size), rng_dis(0, 1000), mutate_dis(0, 200), mutate_ins_dis(0, 400), mutate_rins_dis(0, 800)
    {}
};

#endif