/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FunctionMap.cpp
 * Author: bbermann
 * 
 * Created on 16 de Junho de 2016, 00:41
 */

#include "FunctionMap.hpp"

using namespace std;

FunctionMap::FunctionMap()
{
}

FunctionMap::FunctionMap(const FunctionMap& orig)
{
}

FunctionMap::~FunctionMap()
{
}

void FunctionMap::add(std::string key, FunctionPointer function)
{
    this->insert(make_pair(key, function));
}

void FunctionMap::remove(std::string key)
{
    this->erase(key);
}