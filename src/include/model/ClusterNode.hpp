#pragma once
#include <string>
#include "BaseMoel.hpp"

class ClusterNode : public BaseModel
{
  public:
    int id;
    int clusterId;
    int cores;
    int memory;

    virtual TypeMapping getColumnMappingType(std::string key) override
    {
        return TypeMapping::Integer;
    }
};