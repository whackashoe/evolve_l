#include "settings.hpp"
#include "universe.hpp"



int main(int argc, char ** argv)
{
    Settings settings;
    settings.print_regularity = 10;
    settings.init_train(16);

    Universe universe(settings);
    universe.run();

    return 0;
}