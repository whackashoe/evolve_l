#include "settings.hpp"
#include "universe.hpp"
#include "rng.hpp"



int main(int argc, char ** argv)
{
    Settings settings;
    settings.set_print_regularity(1);
    settings.set_max_iterations(100000);
    settings.set_grammar_size(10);
    settings.set_run_iterations(4);
    settings.set_rule_mutation_probability(50);
    settings.set_rulepart_insdel_probability(100);
    settings.set_rulerow_insdel_probability(200);

    Universe universe(settings);
    universe.init_train(settings, 800, 1500);
    universe.run(settings);

    return 0;
}