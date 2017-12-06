/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DTO.hpp
 * Author: bbermann
 *
 * Created on 6 de Janeiro de 2016, 20:54
 */

#ifndef DTO_HPP
#define DTO_HPP

#include "../type/String.hpp"
#include <vector>

typedef std::pair<String, String> Property;

struct Properties {
    std::vector<Property> propertyVector;
    
    Property Get(String property) {
        for(auto p : propertyVector) {
            if(p.first == property) return p;
        }
        
        throw new std::invalid_argument("DTO::Properties::Get - O argumento " + property + " foi utilizado na pesquisa e não encontrou nenhum registro!");
    }
    
    void Set(String property, String value) {
        for(auto p : propertyVector) {
            if(p.first == property) {
                p.second = value;
                return;
            }
        }
        
        throw new std::invalid_argument("DTO::Properties::Set - O argumento " + property + " foi utilizado na pesquisa e não encontrou nenhum registro!");
    }
};

class DTO {
public:
    DTO();
    virtual ~DTO();
    
    void AddProperty(String property, String value);
    Property GetProperty(String property);
    void SetProperty(String property, String value);
    Properties properties;
    
private:
};

#endif /* DTO_HPP */

