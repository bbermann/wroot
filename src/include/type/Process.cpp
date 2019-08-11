/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Process.cpp
 * Author: bbermann
 *
 * Created on 21 de Fevereiro de 2016, 23:55
 */

#include "Process.hpp"

using namespace std;

Process::Process(const String &executable_path, const String &arguments) {
    this->executable_path_ = executable_path;
    this->arguments_ = arguments;
}

Process::Process(const Process &orig) {
}

Process::~Process() = default;

const char *Process::getCommand() {
    String call = this->executable_path_ + " " + this->arguments_;
    return call.c_str();
}

int Process::run() {
    int status = system(this->getCommand());
    return status;
}

void Process::runAsync() {
    thread asyncCall(&system, this->getCommand());
    asyncCall.detach();
}

String Process::runWithOutput() {
    std::array<char, 128> buffer{};
    std::string result;

    FILE *pipeDescriptor = popen(this->getCommand(), "r");
    std::shared_ptr<FILE> pipe(pipeDescriptor, pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
            result += buffer.data();
        }
    }

    return result;
}