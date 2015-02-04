#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include "rng.hpp"

struct Settings
{
    size_t population_size;  //amount of concurrent systems per generation
    size_t grammar_size;     //variety of grammar to generate (3 would be A B C)
    size_t run_iterations;   //how many iterations should the system run during rewrite stage
    size_t max_iterations;   //how many total iterations should we do
    size_t print_regularity; //how many iterations between printing current status

    Rng rng;

    //default settings here
    Settings() : 
    population_size(32),   
    grammar_size(8),       
    run_iterations(10),    
    max_iterations(100000),
    print_regularity(10),
    rng{}
    {}
    
    void set_population_size(const size_t population_size)
    {
        this->population_size = population_size;
    }

    void set_grammar_size(const size_t grammar_size)
    {
        this->grammar_size = grammar_size;
        rng.grammar_dis.param(decltype(rng.grammar_dis)::param_type(0, grammar_size));
    }

    void set_run_iterations(const size_t run_iterations)
    {
        this->run_iterations = run_iterations;
    }

    void set_max_iterations(const size_t max_iterations)
    {
        this->max_iterations = max_iterations;
    }

    void set_print_regularity(const size_t print_regularity)
    {
        this->print_regularity = print_regularity;
    }

    void set_rule_mutation_probability(const size_t rule_mutation_probability)
    {
        rng.mutate_dis.param(decltype(rng.mutate_dis)::param_type(0, rule_mutation_probability));
    }

    void set_rulepart_insdel_probability(const size_t rule_insdel_probability)
    {
        rng.mutate_ins_dis.param(decltype(rng.mutate_ins_dis)::param_type(0, rule_insdel_probability));
    }

    void set_rulerow_insdel_probability(const size_t rulerow_insdel_probability)
    {
        rng.mutate_rins_dis.param(decltype(rng.mutate_rins_dis)::param_type(0, rulerow_insdel_probability));
    }
};

#endif