/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.hpp
 * Author: bbermann
 *
 * Created on 21 de Fevereiro de 2016, 23:55
 */

#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "../core/Core.hpp"
#include "String.hpp"
#include <thread>

class Process {
public:
    explicit Process(const String &executable_path, const String &arguments = "");

    Process(const Process &orig);

    virtual ~Process();

    int run();

    void runAsync();

    String runWithOutput();

protected:
    const char *getCommand();

private:
    String executable_path_;
    String arguments_;
};

#endif /* PROCESS_HPP */