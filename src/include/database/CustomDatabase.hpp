#pragma once
#include "include/core/Core.hpp"

namespace BBermann::WRoot::Database
{

class ResultSet : public std::vector<std::vector<std::string>>
{
public:

    StringList first()
    {
        if (this->size() > 0) {
            return this->at(0);
        }

        throw std::runtime_error("Undefined index 0 in the ResultSet.");
    }

    StringList last()
    {
        return this->at(this->size() - 1);
    }
};

class CustomDatabase
{
  public:
    CustomDatabase()
    {
    }

    virtual ~CustomDatabase()
    {
    }
    
    // Pure virtuals
    virtual void execute(std::string command) = 0;
    virtual ResultSet query(std::string query) = 0;
};

} // namespace BBermann::WRoot::Database