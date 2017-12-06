/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Exception.hpp"

using namespace std;

Exception::Exception(string message, string location)
{
    this->message_ = message;
    this->where_ = location;
}

Exception::~Exception()
{
    
}

const char* Exception::what() const _GLIBCXX_USE_NOEXCEPT
{
    return this->message_.c_str();
}

const char* Exception::where()
{
    return this->where_.c_str();
}