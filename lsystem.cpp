#include <vector>
#include <iostream>
#include "levenshtein_distance.hpp"
#include "lrule.hpp"
#include "lsystem.hpp"
#include "rng.hpp"


LSystem::LSystem() : rules(), data() {}


bool LSystem::match(const int data_p, const int rule_p) const
{
    auto rule_s = rules[rule_p].from.size();
    if(data_p + rule_s > data.size()) {
        return false;
    }

    for(size_t i=0; i<rule_s; ++i) {
        if(data[data_p + i] != rules[rule_p].from[i]) {
            return false;
        }
    }

    return true;
}

void LSystem::iterate()
{
    std::vector<int> rval;
    rval.reserve(data.size());

    for(size_t i=0; i<data.size(); ) {
        bool found = false;

        for(size_t j=0; j<rules.size(); ++j) {
            auto r = rules[j];

            if(match(i, j)) {
                auto rto = r.to;
                rval.insert(rval.end(), rto.begin(), rto.end());
                found = true;
                i += r.from.size();
                break;
            }
        }

        if(! found) {
            rval.push_back(data[i]);
            ++i;
        }
    }

    data = rval;
}

void LSystem::iterate(const std::vector<int> & axiom, int n)
{
    data = axiom;

    for(int i=0; i<n; ++i) {
        iterate();
    }
}

int LSystem::distance(const std::vector<int> & target) const
{
    return levenshtein_distance(data, target);
}

void LSystem::mutate(Rng & rng)
{
    for(size_t i=0; i<rules.size(); ++i) {
        //from group
        for(size_t j=0; j<rules[i].from.size(); ++j) {
            if(rng.mutate_dis(rng.gen) == 0) {
                rules[i].from[j] = rng.grammar_dis(rng.gen);
            }
        }

        if(rng.mutate_ins_dis(rng.gen) == 0) {
            rules[i].from.emplace(rules[i].from.begin() + (rng.rng_dis(rng.gen) % rules[i].from.size()), rng.grammar_dis(rng.gen));
        }

        if(rules[i].from.size() > 1 && rng.mutate_ins_dis(rng.gen) == 0) {
            rules[i].from.erase(rules[i].from.begin() + (rng.rng_dis(rng.gen) % rules[i].from.size()));
        }

        //to group
        for(size_t j=0; j<rules[i].to.size(); ++j) {
            if(rng.mutate_dis(rng.gen) == 0) {
                rules[i].to[j] = rng.grammar_dis(rng.gen);
            }
        }

        if(rng.mutate_ins_dis(rng.gen) == 0) {
            rules[i].to.emplace(rules[i].to.begin() + (rng.rng_dis(rng.gen) % rules[i].to.size()), rng.grammar_dis(rng.gen));
        }

        if(rules[i].to.size() > 1 && rng.mutate_ins_dis(rng.gen) == 0) {
            rules[i].to.erase(rules[i].to.begin() + (rng.rng_dis(rng.gen) % rules[i].to.size()));
        }
    }

    //rules group
    if(rng.mutate_rins_dis(rng.gen) == 0) {
        rules.emplace(rules.begin() + (rng.rng_dis(rng.gen) % rules.size()), LRule{ rng.gen, rng.rng_dis, rng.grammar_dis });
    }

    if(rules.size() > 1 && rng.mutate_rins_dis(rng.gen) == 0) {
        rules.erase(rules.begin() + (rng.rng_dis(rng.gen) % rules.size()));
    }
}

LSystem LSystem::generate_random_lsystem(Rng & rng)
{
    LSystem lsys;

    do {
        lsys.rules.push_back(LRule{ rng.gen, rng.rng_dis, rng.grammar_dis });
    } while(rng.rng_dis(rng.gen) < 500);

    return lsys;
}

LSystem LSystem::sexual_reproduction(const LSystem & a, const LSystem & b, Rng & rng)
{
    LSystem lsys;

    size_t rules_size = std::min(a.rules.size(), b.rules.size());

    for(size_t i=0; i<rules_size; ++i) {
        lsys.rules.push_back(i % 2 == 0 ? a.rules[i] : b.rules[i]);
    }

    lsys.mutate(rng);

    return lsys;
}
