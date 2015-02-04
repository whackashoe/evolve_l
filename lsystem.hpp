#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <vector>
#include <iostream>
#include "utility.hpp"
#include "lrule.hpp"
#include "rng.hpp"

/*
 * special lsystem
 * this one allows for multiple predicates to a rule
 * and has mutation / reproductive ability
 * read more here: http://en.wikipedia.org/wiki/L-system
*/
struct LSystem
{
    std::vector<LRule> rules;
    std::vector<int> data;

    LSystem();

    friend std::ostream& operator<< (std::ostream& stream, const LSystem& lsys);

    bool match(const int data_p, const int rule_p) const;

    void iterate();

    void iterate(const std::vector<int> & axiom, const int n);
    
    void iterate_print(const std::vector<int> & axiom, const int n);

    int distance(const std::vector<int> & target) const;

    void mutate(Rng & rng);

    static LSystem generate_random_lsystem(Rng & rng);

    static LSystem sexual_reproduction(const LSystem & a, const LSystem & b, Rng & rng);
};

#endif