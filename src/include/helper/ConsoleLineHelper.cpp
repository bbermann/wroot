#include "ConsoleLineHelper.hpp"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

ConsoleLineHelper::ConsoleLineHelper(String command) {
    command_ = command;
}

ConsoleLineHelper::~ConsoleLineHelper() {
}

String ConsoleLineHelper::executeStdOut() {
    std::array<char, 128> buffer;
    std::string result;

    std::shared_ptr <FILE> pipe(popen(command_.c_str(), "r"), pclose);

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
            result += buffer.data();
        }
    }

    return result;
}
