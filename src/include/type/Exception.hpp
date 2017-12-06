/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Exceptions.hpp
 * Author: bbermann
 *
 * Created on 25 de Fevereiro de 2016, 01:50
 */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <stdexcept>
#include "String.hpp"

#ifndef _GLIBCXX_USE_NOEXCEPT
    #define _GLIBCXX_USE_NOEXCEPT
#endif

class Exception : public std::exception
{
public:

    Exception(std::string message, std::string location);
    virtual ~Exception();
    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override;
    virtual const char* where();

protected:
    std::string message_, where_;
};

#endif /* EXCEPTION_HPP */
