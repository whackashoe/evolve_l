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


double median(std::vector<int> vals)
{
    size_t size = vals.size();
    sort(vals.begin(), vals.end());
    return (size % 2 == 0) ? (vals[size / 2 - 1] + vals[size / 2]) / 2 : vals[size / 2]; 
}

int main(int argc, char ** argv)
{
    Settings settings {};
    Rng rng { settings.grammar_size };
    settings.init_train(rng);

    settings.print_input_and_target();

    std::vector<LSystem> population;
    LSystem rls = LSystem::generate_random_lsystem(rng);
    for(int i=0; i<settings.population_size; ++i) {
        rls.mutate(rng);
        population.push_back(rls);
    }

    for(int i=0; i<settings.max_iterations; ++i) {
        std::vector<int> distances;

        for(int j=0; j<population.size(); ++j) {
            population[j].iterate(settings.input, 5);
            distances.push_back(population[j].distance(settings.target));
        }

        float average_distance = median(distances);

        auto min_distance = std::min_element(distances.begin(), distances.end());
        auto best = population[std::distance(distances.begin(), min_distance)];

        for(int cur=0; cur<population.size(); ++cur) {
            if(distances[cur] > average_distance) {
                population[cur] = best;
            } else {
                int mate_pos;
                do {
                    mate_pos = rng.rng_dis(rng.gen) % distances.size();
                } while(mate_pos != cur && distances[mate_pos] > average_distance);

                population[cur] = LSystem::sexual_reproduction(population[cur], population[mate_pos], rng);
            }
        }


        if(i % 1000 == 0) {
            std::cout << i << "\t" << *min_distance << "\t" << average_distance << std::endl;
            std::cout << best << std::endl;
        }
    }

    return 0;
}