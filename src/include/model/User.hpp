#pragma once

#include <string>
#include "BaseModel.hpp"

class User : public BaseModel {
public:
    int id;
    std::string name;
    std::string token;

    virtual TypeMapping getColumnMappingType(std::string key) override {
        if (key == "id")
            return TypeMapping::Integer;
        else if (key == "name")
            return TypeMapping::Text;
        else if (key == "token")
            return TypeMapping::Text;
        else if (key == "token")
            return TypeMapping::Text;
        return TypeMapping::Unknown;
    }
};