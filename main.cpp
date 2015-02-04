#include <vector>
#include <iostream>
#include <random>
#include <functional>
#include <algorithm>
#include <cassert>

std::vector<int> input;
std::vector<int> target;

size_t population_size { 20 };
size_t grammar_size { 10 };
size_t run_iterations { 50 };
size_t max_iterations { 10000000 };
size_t train_length { 100 };

std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_int_distribution<int> grammar_dis(0, grammar_size);
std::uniform_int_distribution<int> rng_dis(0, 1000);
std::uniform_int_distribution<int> mutate_dis(0, 200);
std::uniform_int_distribution<int> mutate_ins_dis(0, 400);
std::uniform_int_distribution<int> mutate_rins_dis(0, 800);


//http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C.2B.2B
template<class T>
unsigned int levenshtein_distance(const T &s1, const T & s2) {
    const size_t len1 = s1.size(), len2 = s2.size();
    std::vector<unsigned int> col(len2+1), prevCol(len2+1);
 
    for (unsigned int i = 0; i < prevCol.size(); i++)
        prevCol[i] = i;
    for (unsigned int i = 0; i < len1; i++) {
        col[0] = i+1;
        for (unsigned int j = 0; j < len2; j++)
            col[j+1] = std::min( std::min(prevCol[1 + j] + 1, col[j] + 1),
                                prevCol[j] + (s1[i]==s2[j] ? 0 : 1) );
        col.swap(prevCol);
    }
    return prevCol[len2];
}



class LRule
{
public:
    std::vector<int> from;
    std::vector<int> to;

    LRule(std::vector<int> from, std::vector<int> to)
    {
        this->from = from;
        this->to = to;
    }
};

class LSystem
{
public:
    std::vector<LRule> rules;
    std::vector<int> data;

    LSystem() {}

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


    bool match(int data_p, int rule_p)
    {
        auto rule_s = rules[rule_p].from.size();
        if(data_p + rule_s > data.size()) {
            return false;
        }

        for(int i=0; i<rule_s; ++i) {
            if(data[data_p + i] != rules[rule_p].from[i]) {
                return false;
            }
        }

        return true;
    }

    void iterate()
    {
        std::vector<int> rval;

        for(int i=0; i<data.size(); ) {
            bool found = false;

            for(int j=0; j<rules.size(); ++j) {
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

    void iterate(const std::vector<int> axiom, int n)
    {
        data = axiom;

        for(int i=0; i<n; ++i) {
            iterate();
        }
    }

    std::vector<int> result()
    {
        return data;
    }

    int distance()
    {
        return levenshtein_distance(data, target);
    }
};

LRule generate_random_lrule()
{
    std::vector<int> r_from;
    std::vector<int> r_to;

    do {
        r_from.push_back(grammar_dis(gen));
    } while(rng_dis(gen) < 200);

    do {
        r_to.push_back(grammar_dis(gen));
    } while(rng_dis(gen) < 200);

    return LRule(r_from, r_to);
}

LSystem generate_random_lsystem()
{
    LSystem lsys;

    do {
        lsys.rules.push_back(generate_random_lrule());
    } while(rng_dis(gen) < 500);

    return lsys;
}

LSystem mutate(LSystem lsys)
{
    for(int i=0; i<lsys.rules.size(); ++i) {
        //from group
        for(int j=0; j<lsys.rules[i].from.size(); ++j) {
            if(mutate_dis(gen) == 0) {
                lsys.rules[i].from[j] = grammar_dis(gen);
            }
        }

        if(mutate_ins_dis(gen) == 0) {
            lsys.rules[i].from.emplace(lsys.rules[i].from.begin() + (rng_dis(gen) % lsys.rules[i].from.size()), grammar_dis(gen));
        }

        if(lsys.rules[i].from.size() > 1 && mutate_ins_dis(gen) == 0) {
            lsys.rules[i].from.erase(lsys.rules[i].from.begin() + (rng_dis(gen) % lsys.rules[i].from.size()));
        }

        //to group
        for(int j=0; j<lsys.rules[i].to.size(); ++j) {
            if(mutate_dis(gen) == 0) {
                lsys.rules[i].to[j] = grammar_dis(gen);
            }
        }

        if(mutate_ins_dis(gen) == 0) {
            lsys.rules[i].to.emplace(lsys.rules[i].to.begin() + (rng_dis(gen) % lsys.rules[i].to.size()), grammar_dis(gen));
        }

        if(lsys.rules[i].to.size() > 1 && mutate_ins_dis(gen) == 0) {
            lsys.rules[i].to.erase(lsys.rules[i].to.begin() + (rng_dis(gen) % lsys.rules[i].to.size()));
        }
    }

    //rules group
    if(mutate_rins_dis(gen) == 0) {
        lsys.rules.emplace(lsys.rules.begin() + (rng_dis(gen) % lsys.rules.size()), generate_random_lrule());
    }

    if(lsys.rules.size() > 1 && mutate_rins_dis(gen) == 0) {
        lsys.rules.erase(lsys.rules.begin() + (rng_dis(gen) % lsys.rules.size()));
    }

    return lsys;
}

LSystem sexual_reproduction(LSystem a, LSystem b)
{
    LSystem lsys;

    int rules_size = std::min(a.rules.size(), b.rules.size());

    for(int i=0; i<rules_size; ++i) {
        lsys.rules.push_back(i % 2 == 0 ? a.rules[i] : b.rules[i]);
    }

    return mutate(lsys);
}

int main(int argc, char ** argv)
{
    for(int i=0; i<train_length; ++i) {
        input.push_back(grammar_dis(gen));
        target.push_back(grammar_dis(gen));
    }

    std::cout << "INPUT:  \t";
    for(int i=0; i<input.size(); ++i) {
        std::cout << input[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "TARGET: \t";
    for(int i=0; i<target.size(); ++i) {
        std::cout << target[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;


    assert(population_size > 0);
    assert(grammar_size > 0);
    assert(run_iterations > 0);
    assert(max_iterations > 0);
    assert(train_length > 0);


    std::vector<LSystem> population;
    LSystem rls = generate_random_lsystem();
    for(int i=0; i<population_size; ++i) {
        population.push_back(mutate(rls));
    }

    for(int i=0; i<max_iterations; ++i) {
        std::vector<int> distances;

        for(int j=0; j<population.size(); ++j) {
            population[j].iterate(input, 5);
            distances.push_back(population[j].distance());
        }

        float average_distance = std::accumulate(distances.begin(), distances.end(), 0.0) / distances.size();

        auto attractive_mate = [&](int n) {
            int i;
            do {
                i = rng_dis(gen) % distances.size();
            } while(i != n && distances[i] > average_distance);

            return population[i];
        };

        auto min_distance = std::min_element(distances.begin(), distances.end());
        auto best = population[std::distance(distances.begin(), min_distance)];

        for(int j=0; j<population.size(); ++j) {
            if(distances[j] > average_distance) {
                population[j] = best;
            } else {
                population[j] = sexual_reproduction(population[j], attractive_mate(j));
            }
        }


        if(i % 1000 == 0) {
            std::cout << i << "\t" << *min_distance << "\t" << average_distance << std::endl;
            std::cout << best << std::endl;
        }
        /*
        auto best = population[std::distance(distances.begin(), min_distance)];
        for(int k=0; k<best.data.size(); ++k) {
            std::cout << best.data[k] << " ";
        }
        std::cout << std::endl;*/
    }

    return 0;
}