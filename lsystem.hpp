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

    friend std::ostream& operator<< (std::ostream& stream, const LSystem& lsys)
    {
        for(int i=0; i<lsys.rules.size(); ++i) {
            stream << "FROM: \t";
            for(int j=0; j<lsys.rules[i].from.size(); ++j) {
                stream << lsys.rules[i].from[j] << " ";
            }
            stream << std::endl;

            stream << "TO: \t";
            for(int j=0; j<lsys.rules[i].to.size(); ++j) {
                stream << lsys.rules[i].to[j] << " ";
            }
            stream << std::endl;
        }

        for(int i=0; i<lsys.data.size(); ++i) {
            stream << lsys.data[i] << " ";
        }

        stream << std::endl;

        return stream;
    }


    bool match(int data_p, int rule_p);

    void iterate();

    void iterate(const std::vector<int> axiom, int n);

    int distance(const std::vector<int> & target);

    void mutate(Rng & rng);

    static LSystem generate_random_lsystem(Rng & rng);

    static LSystem sexual_reproduction(const LSystem & a, const LSystem & b, Rng & rng);
};

#endif