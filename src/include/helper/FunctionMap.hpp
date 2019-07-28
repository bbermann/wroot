/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FunctionMap.hpp
 * Author: bbermann
 *
 * Created on 16 de Junho de 2016, 00:41
 */

#ifndef FUNCTIONMAP_HPP
#define FUNCTIONMAP_HPP

#include "../type/String.hpp"

#include <map>

typedef void (*FunctionPointer)();

typedef std::map <std::string, FunctionPointer> FunctionMap_;

class FunctionMap : public FunctionMap_ {
public:
    FunctionMap();

    FunctionMap(const FunctionMap &orig);

    virtual ~FunctionMap();

    void add(std::string key, FunctionPointer function);

    void remove(std::string key);

private:
};

#endif /* FUNCTIONMAP_HPP */

