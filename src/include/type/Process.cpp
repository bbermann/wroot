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
#include "Exception.hpp"

using namespace std;

Process::Process(String executable_path, String arguments)
{
    this->executable_path_ = executable_path;
    this->arguments_ = arguments;
}

Process::Process(const Process& orig)
{
}

Process::~Process()
{
}

const char* Process::getCommand()
{
    String call = this->executable_path_ + " " + this->arguments_;
    return call.c_str();
}

int Process::run()
{
    int status = system(getCommand());
    return status;
}

void Process::runAsync()
{

    thread asyncCall(&system, getCommand());
    asyncCall.detach();
}