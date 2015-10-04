#ifndef RNG_H
#define RNG_H

#include <random>

struct Rng
{
    std::random_device rd;
    std::mt19937_64 gen;
    std::uniform_int_distribution<int> grammar_dis;     //to get random grammar piece within our grammar size
    std::uniform_int_distribution<int> rng_dis;         //general purpose random generator
    std::uniform_int_distribution<int> mutate_dis;      //how often to mutate a piece of a rule
    std::uniform_int_distribution<int> mutate_ins_dis;  //how often should we create or delete a piece of a rule
    std::uniform_int_distribution<int> mutate_rins_dis; //how often should we create or delete a rule
    std::uniform_int_distribution<int> binary_dis;      //binary rng rule

    Rng() : 
    rd(),
    gen(rd()),
    grammar_dis(0, 8),
    rng_dis(0, 1000),
    mutate_dis(0, 200),
    mutate_ins_dis(0, 400),
    mutate_rins_dis(0, 800),
    binary_dis(0, 1)
    {}
};

#endif