#include "Individual.hpp"
#include "RandomNum.hpp"

Individual::Individual(const std::string &chromosome) {
    this->chromosome = chromosome;
    fitness = calibrateFitness();
}

// Perform mating and produce new offspring
Individual Individual::mate(Individual partner) {
    // chromosome for offspring
    std::string child_chromosome = "";

    int len = chromosome.size();
    for (int i = 0; i < len; i++) {
        // random probability
        float p = RandomNum::random(0, 100) / 100;

        // if prob is less than 0.45, insert gene
        // from parent 1
        if (p < 0.45)
            child_chromosome += chromosome[i];

            // if prob is between 0.45 and 0.90, insert
            // gene from parent 2
        else if (p < 0.90)
            child_chromosome += partner.chromosome[i];

            // otherwise insert random gene(mutate),
            // for maintaining diversity
        else
            child_chromosome += RandomNum::mutatedGenes();
    }

    // create new Individual(offspring) using
    // generated chromosome for offspring
    return Individual(child_chromosome);
}


// Calculate fittness score, it is the number of
// characters in string which differ from target
// string.
int Individual::calibrateFitness() {
    int len = RandomNum::target.size();
    int fitness = 0;

    for (int i = 0; i < len; i++) {
        if (chromosome[i] != RandomNum::target[i])
            fitness++;
    }

    return fitness;
}