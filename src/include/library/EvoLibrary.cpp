#include "EvoLibrary.hpp"
#include "EvoLibrary/Individual.hpp"
#include "EvoLibrary/RandomNum.hpp"

// C++ program to create target string, starting from
// random string using Genetic Algorithm

// Number of individuals in each generation
#define POPULATION_SIZE 100

EvoLibrary::EvoLibrary(const Request &request) : CustomLibrary(request) {

}

void EvoLibrary::handle(Response &response) {
    std::stringstream responseStream;

    responseStream << "<html><body><pre>";

    RandomNum::seed();

    // current generation
    int generation = 0;
    bool found = false;
    std::vector<Individual> population;

    // create initial population
    for (int i = 0; i < POPULATION_SIZE; i++) {
        std::string gnome = this->createGenome();
        population.emplace_back(gnome);
    }

    do {
        // sort the population in increasing order of fitness score
        sort(population.begin(), population.end());

        // if the individual having lowest fitness score ie.
        // 0 then we know that we have reached to the target
        // and break the loop
        if (population[0].fitness <= 0) {
            found = true;
            break;
        }

        // Otherwise generate new offsprings for new generation
        std::vector<Individual> new_generation;

        // Perform Elitism, that mean 10% of fittest population
        // goes to the next generation
        int s = (10 * POPULATION_SIZE) / 100;

        for (int i = 0; i < s; i++) {
            new_generation.push_back(population[i]);
        }

        // From 50% of fittest population, Individuals
        // will mate to produce offspring
        s = (90 * POPULATION_SIZE) / 100;

        for (int i = 0; i < s; i++) {
            int len = population.size();
            int r = RandomNum::random(0, 50);
            Individual parent1 = population[r];
            r = RandomNum::random(0, 50);
            Individual parent2 = population[r];
            Individual offspring = parent1.mate(parent2);
            new_generation.push_back(offspring);
        }

        population = new_generation;

        responseStream << "Generation: " << generation << "\t";
        responseStream << "String: " << population[0].chromosome << "\t";
        responseStream << "Fitness: " << population[0].fitness << "\n";

        generation++;
    } while (!found);

    responseStream << "Generation: " << generation << "\t";
    responseStream << "String: " << population[0].chromosome << "\t";
    responseStream << "Fitness: " << population[0].fitness << "\n";

    responseStream << "</pre></body></html>";

    response.status = Response::Ok;
    response.content = responseStream.str();
    response.headers.resize(2);
    response.headers[0].name = "Content-Length";
    response.headers[0].value = std::to_string(response.content.size());
    response.headers[1].name = "Content-Type";
    response.headers[1].value = "text/html";
}

// create chromosome or string of genes
std::string EvoLibrary::createGenome() {
    int len = RandomNum::target.size();
    std::string gnome{};
    for (int i = 0; i < len; i++)
        gnome += RandomNum::mutatedGenes();
    return gnome;
}