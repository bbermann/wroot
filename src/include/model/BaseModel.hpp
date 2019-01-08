#pragma once
#include "include/type/String.hpp"
#include <map>

class BaseModel
{
public:
    BaseModel()
    {

    }

    virtual ~BaseModel();

    virtual void* get(std::string key)
    {
        return attributes_.at(key);
    }

protected:
    std::map<std::string, void*> attributes_;
};