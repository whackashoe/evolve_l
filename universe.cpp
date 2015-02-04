#include <cmath>
#include "universe.hpp"
#include "utility.hpp"

//initializes the INPUT and TARGET fields with random data
//of train_length length. 
//alternatively we could set these manually in main
void Universe::init_train(Settings & settings, const size_t train_length)
{
    for(size_t i=0; i<train_length; ++i) {
        input.push_back(settings.rng.grammar_dis(settings.rng.gen));
        target.push_back(settings.rng.grammar_dis(settings.rng.gen));
    }
}

void Universe::init_train(Settings & settings, const size_t input_length, const size_t target_length)
{
    for(size_t i=0; i<input_length; ++i) {
        input.push_back(settings.rng.grammar_dis(settings.rng.gen));
    }
    for(size_t i=0; i<target_length; ++i) {
        target.push_back(settings.rng.grammar_dis(settings.rng.gen));
    }
}


void Universe::populate_universe(Settings & settings)
{
    LSystem rls = LSystem::generate_random_lsystem(settings.rng);
    for(size_t i=0; i<settings.population_size; ++i) {
        rls.mutate(settings.rng);
        population.push_back(rls);
    }
}


//print out our input and target fields we wish to match to
void Universe::print_input_and_target()
{
    std::cout << "INPUT: "  << std::endl;
    for(size_t i=0; i<input.size(); ++i) {
        std::cout << (char) ('A' + input[i]) << " ";
    }
    std::cout << std::endl;

    std::cout << "TARGET: " << std::endl;
    for(size_t i=0; i<target.size(); ++i) {
        std::cout << (char) ('A' + target[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void Universe::run(Settings & settings)
{
    assert("you probably don't intend for your input to be totally empty, try calling: `universe.init_train()` to populate with random data" && input.size() != 0);

    //store how far away from target each system in population is
    std::vector<int> distances(population.size());


    //well try to find perfect solution up until max_iterations
    for(size_t i=0; i<settings.max_iterations; ++i) {
        //perform run through
        for(size_t j=0; j<population.size(); ++j) {
            population[j].iterate(input, settings.run_iterations);
            distances[j] = population[j].distance(target);
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
            alpha.iterate_print(input, settings.run_iterations);
            break;
        }
    }

    //as a courtesy show the full results
    population[0].iterate_print(input, settings.run_iterations);

    //show the input and target results again for comparison
    print_input_and_target();
}