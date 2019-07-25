#pragma once
#include <string>
#include "BaseModel.hpp"

class Cluster : public BaseModel
{
  public:
    int id;
    int userId;
    std::string name;

    virtual TypeMapping getColumnMappingType(std::string key) override
    {
        if (key == "id")
            return TypeMapping::Integer;
        else if (key == "userId")
            return TypeMapping::Integer;
        else if (key == "name")
            return TypeMapping::Text;
            
        return TypeMapping::Unknown;
    }
};