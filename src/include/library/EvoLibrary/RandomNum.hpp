#pragma once

#include <bits/stdc++.h>
#include <iostream>

namespace RandomNum {
    // Valid Genes
    const std::string genes = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890, .-;:_!\"#%&/()=?@${[]}";

    // Target string to be generated
    const std::string target = "MINHA CHAVE ULTRA SECRETA QUE NENHUMA INTELIGENCIA ARTIFICIAL VAI QUEBRAR";

    void seed();

    // Function to generate random numbers in given range
    int random(int start, int end);

    // Create random genes for mutation
    char mutatedGenes();
}
