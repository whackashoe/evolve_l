#include <vector>
#include <iostream>
#include "utility.hpp"
#include "lrule.hpp"
#include "lsystem.hpp"
#include "rng.hpp"


LSystem::LSystem() : rules(), data() {}

//just a way to print stuff out easily
std::ostream& operator<< (std::ostream& stream, const LSystem& lsys)
{
    for(size_t i=0; i<lsys.rules.size(); ++i) {
        stream << "FROM: \t";
        for(size_t j=0; j<lsys.rules[i].from.size(); ++j) {
            stream << (char) ('A' + lsys.rules[i].from[j]) << " ";
        }
        stream << std::endl;

        stream << "TO: \t";
        for(size_t j=0; j<lsys.rules[i].to.size(); ++j) {
            stream << (char) ('A' + lsys.rules[i].to[j]) << " ";
        }
        stream << std::endl;
    }

    for(size_t i=0; i<lsys.data.size(); ++i) {
        stream << (char) ('A' + lsys.data[i]) << " ";
    }

    stream << std::endl;

    return stream;
}

//does a rule match our current position?
bool LSystem::match(const int data_p, const int rule_p) const
{
    auto rule_s = rules[rule_p].from.size();
    //if the rule from size in addition to our current data position is larger than the overall data size....
    if(data_p + rule_s > data.size()) {
        return false;
    }

    //check that each predicate for rule matches
    for(size_t i=0; i<rule_s; ++i) {
        if(data[data_p + i] != rules[rule_p].from[i]) {
            return false;
        }
    }

    //they do match :D
    return true;
}

void LSystem::iterate()
{
    std::vector<int> buf;
    buf.reserve(data.size());

    //run through each bit of our data
    for(size_t i=0; i<data.size(); ) {
        bool found = false;

        //check all rules to see if we can apply them
        for(size_t j=0; j<rules.size(); ++j) {
            auto r = rules[j];

            if(match(i, j)) {
                //insert rules TO data to the end of our buffer and skip ahead
                buf.insert(buf.end(), r.to.begin(), r.to.end());
                found = true;
                //skip ahead however many FROM size
                i += r.from.size(); //i++; //we could also do this?
                break;
            }
        }

        //if no rules matches, we just add data back here
        if(! found) {
            buf.push_back(data[i]);
            ++i;
        }
    }

    //set our internal data to our buffer
    data = buf;
}

//utility function
void LSystem::iterate(const std::vector<int> & axiom, const int n)
{
    data = axiom;

    for(int i=0; i<n; ++i) {
        iterate();
    }
}

//utility function
void LSystem::iterate_print(const std::vector<int> & axiom, const int n)
{
    data = axiom;

    for(int i=0; i<n; ++i) {
        iterate();
        for(size_t i=0; i<data.size(); ++i) {
            std::cout << (char) ('A' + data[i]) << " ";
        }
        std::cout << std::endl;
    }
}

//figure out how far away we are from our goal
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

//splices two lsystems together
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
