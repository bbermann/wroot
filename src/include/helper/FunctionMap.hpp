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

typedef std::map <String, FunctionPointer> FunctionMap_t;

class FunctionMap : public FunctionMap_t {
public:
    FunctionMap();

    virtual ~FunctionMap();

    void add(const String &key, FunctionPointer function);

    void remove(const String &key);

private:
};

#endif /* FUNCTIONMAP_HPP */

