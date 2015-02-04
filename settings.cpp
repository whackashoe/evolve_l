#include "settings.hpp"
#include "rng.hpp"

//initializes the INPUT and TARGET fields with random data
//of train_length length. 
//alternatively we could set these manually in main
void Settings::init_train(const size_t train_length)
{
    Rng rng;
    for(size_t i=0; i<train_length; ++i) {
        input.push_back(rng.grammar_dis(rng.gen));
        target.push_back(rng.grammar_dis(rng.gen));
    }
}

void Settings::init_train(const size_t input_length, const size_t target_length)
{
    Rng rng;
    for(size_t i=0; i<input_length; ++i) {
        input.push_back(rng.grammar_dis(rng.gen));
    }
    for(size_t i=0; i<target_length; ++i) {
        target.push_back(rng.grammar_dis(rng.gen));
    }
}



//print out our input and target fields we wish to match to
void Settings::print_input_and_target()
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