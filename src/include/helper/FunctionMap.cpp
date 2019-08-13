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

FunctionMap::FunctionMap() = default;

FunctionMap::~FunctionMap() = default;

void FunctionMap::add(const String &key, FunctionPointer function) {
    this->insert(make_pair(key, function));
}

void FunctionMap::remove(const String &key) {
    this->erase(key);
}