#include "settings.hpp"
#include "universe.hpp"
#include "rng.hpp"



int main(int argc, char ** argv)
{
    Settings settings;
    settings.set_print_regularity(100);
    settings.set_max_iterations(100000);
    settings.set_grammar_size(8);
    settings.set_run_iterations(2);
    settings.set_rule_mutation_probability(40);
    settings.set_rulepart_insdel_probability(80);
    settings.set_rulerow_insdel_probability(160);

    settings.set_population_size(128);

    Universe universe(settings);
    universe.init_train(settings, 10, 10);
    universe.run(settings);

    return 0;
}