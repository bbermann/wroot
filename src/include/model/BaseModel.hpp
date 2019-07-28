#pragma once
#include "include/type/String.hpp"
#include "include/database/TypeMapping.hpp"
#include <map>
#include <type_traits>

using BBermann::WRoot::Database::TypeMapping;

class BaseModel
{
public:
    BaseModel()
    {

    }

    virtual ~BaseModel()
    {

    }

    template<typename T>
    T& get(std::string key)
    {
        return static_cast<T&>(attributes_.at(key));
    }

    // TODO: Remove template and insert in vector specialized, pointing the second value of  attributes_ to this specialized vector address
    template<typename T>
    void set(std::string key, T& value)
    {
        attributes_[key] = static_cast<void*>(value);
    }

    void set(std::string key, const char* value)
    {
        char* copy = new char[strlen(value)];
        attributes_[key] = static_cast<void*>(strcpy(copy, value));
    }

    virtual TypeMapping getColumnMappingType(std::string key)
    {
        return TypeMapping::Unknown;
    }

protected:
    std::map<std::string, void*> attributes_;
    std::map<std::string, TypeMapping> attributesTypeMapping_;
};