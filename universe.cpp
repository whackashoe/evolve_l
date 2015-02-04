#include <cmath>
#include "universe.hpp"
#include "utility.hpp"

void Universe::populate_universe(Settings & settings)
{
    LSystem rls = LSystem::generate_random_lsystem(settings.rng);
    for(size_t i=0; i<settings.population_size; ++i) {
        rls.mutate(settings.rng);
        population.push_back(rls);
    }
}

void Universe::run(Settings & settings)
{
    //store how far away from target each system in population is
    std::vector<int> distances(population.size());


    //well try to find perfect solution up until max_iterations
    for(size_t i=0; i<settings.max_iterations; ++i) {
        //perform run through
        for(size_t j=0; j<population.size(); ++j) {
            population[j].iterate(settings.input, settings.run_iterations);
            distances[j] = population[j].distance(settings.target);
        }

        const double average_distance = mean(distances);
        const auto min_distance_it    = std::min_element(distances.begin(), distances.end());
        const double min_distance     = *min_distance_it;
        //this is the best performing one, the alpha
        LSystem alpha = population[std::distance(distances.begin(), min_distance_it)];

        //perform mating ritual
        for(size_t cur=0; cur<population.size(); ++cur) {
            //for all the ones who are worse than the average, they die
            //the alpha asexually takes their slot
            if(distances[cur] > average_distance) {
                population[cur] = alpha;
            } else {
                //everyone else mates with someone else who is above average
                //tough world, eh?
                size_t mate_pos;
                do {
                    mate_pos = settings.rng.rng_dis(settings.rng.gen) % distances.size();
                } while(mate_pos != cur && distances[mate_pos] > average_distance);

                population[cur] = LSystem::sexual_reproduction(population[cur], population[mate_pos], settings.rng);
            }
        }

        //print out our status
        if(min_distance == 0 || i % settings.print_regularity == 0) {
            double nice_dist = average_distance != 0 ? std::sqrt(1.0 / average_distance) : 1.0;
            std::cout << i << "\t" << min_distance << "\t" << nice_dist << std::endl;
            std::cout << alpha << std::endl;
        }

        //we should exit on perfect success
        if(min_distance == 0) {
            std::cout << "solution found!" << std::endl << std::endl;
            alpha.iterate_print(settings.input, settings.run_iterations);
            break;
        }
    }

    //as a courtesy show the full results
    population[0].iterate_print(settings.input, settings.run_iterations);

    //show the input and target results again for comparison
    settings.print_input_and_target();
}