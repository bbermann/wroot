#include "RandomNum.hpp"

void RandomNum::seed() {
    srand((unsigned) (time(0)));
}

// Function to generate random numbers in given range
int RandomNum::random(int start, int end) {
    int range = (end - start) + 1;
    int random_int = start + (rand() % range);
    return random_int;
}

// Create random genes for mutation
char RandomNum::mutatedGenes() {
    int len = genes.size();
    int r = RandomNum::random(0, len - 1);
    return genes[r];
}