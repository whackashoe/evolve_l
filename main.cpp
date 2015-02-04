#include "settings.hpp"
#include "universe.hpp"
    #include "rng.hpp"



int main(int argc, char ** argv)
{
    Settings settings;
    settings.print_regularity = 100;
    settings.max_iterations = 100000;
    settings.grammar_size = 14;
    settings.init_train(40, 25);

    Universe universe(settings);
    universe.run();

    return 0;
}