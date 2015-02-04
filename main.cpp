#include <vector>
#include <iostream>
#include <random>
#include <functional>
#include <algorithm>
#include <cassert>
#include "lrule.hpp"
#include "levenshtein_distance.hpp"
#include "lsystem.hpp"
#include "rng.hpp"
#include "settings.hpp"


double mean(std::vector<int> vals)
{
    double rval { 0.0 };
    for(size_t i=0; i<vals.size(); ++i) rval += vals[i];
    return rval / vals.size();
}


int main(int argc, char ** argv)
{
    Settings settings {};
    settings.grammar_size = 3;
    settings.population_size = 32;

    Rng rng { settings.grammar_size };
    settings.init_train(rng, 60);

    settings.print_input_and_target();

    std::vector<LSystem> population;
    LSystem rls = LSystem::generate_random_lsystem(rng);
    for(size_t i=0; i<settings.population_size; ++i) {
        rls.mutate(rng);
        population.push_back(rls);
    }

    for(size_t i=0; i<settings.max_iterations; ++i) {
        std::vector<int> distances(population.size());

        for(size_t j=0; j<population.size(); ++j) {
            population[j].iterate(settings.input, settings.run_iterations);
        }

        for(size_t j=0; j<population.size(); ++j) {
            distances[j] = population[j].distance(settings.target);
        }

        float average_distance = mean(distances);

        auto min_distance = std::min_element(distances.begin(), distances.end());
        auto best = population[std::distance(distances.begin(), min_distance)];

        for(size_t cur=0; cur<population.size(); ++cur) {
            if(distances[cur] > average_distance) {
                population[cur] = best;
            } else {
                size_t mate_pos;
                do {
                    mate_pos = rng.rng_dis(rng.gen) % distances.size();
                } while(mate_pos != cur && distances[mate_pos] > average_distance);

                population[cur] = LSystem::sexual_reproduction(population[cur], population[mate_pos], rng);
            }
        }


        if(i % 100 == 0) {
            std::cout << i << "\t" << *min_distance << "\t" << average_distance << std::endl;
            std::cout << best << std::endl;
        }
    }

    return 0;
}