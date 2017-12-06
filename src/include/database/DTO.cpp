/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DTO.cpp
 * Author: bbermann
 * 
 * Created on 6 de Janeiro de 2016, 20:54
 */

#include "DTO.hpp"

DTO::DTO() {
}

DTO::~DTO() {
}

void DTO::AddProperty(String property, String value) {
    Property prop(property, value);
    properties.propertyVector.insert(properties.propertyVector.end(), prop);
}

Property DTO::GetProperty(String property) {
    return properties.Get(property);
}

void DTO::SetProperty(String property, String value) {
    properties.Set(property, value);
}