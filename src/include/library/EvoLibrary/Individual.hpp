#pragma once

#include <iostream>

// Class representing individual in population
class Individual {
public:
    explicit Individual(const std::string &chromosome);

    Individual mate(Individual partner);

    int calibrateFitness();

    // Overloading < operator
    bool operator<(const Individual &individual) {
        return this->fitness < individual.fitness;
    }

    std::string chromosome;
    int fitness;
};


