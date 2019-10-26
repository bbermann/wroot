#ifndef ENVIRONMENTBUILDER_HPP
#define ENVIRONMENTBUILDER_HPP

#include "../type/String.hpp"
#include <map>

class EnvironmentBuilder {
public:
    EnvironmentBuilder();

    ~EnvironmentBuilder();

    void setPair(String key, String value);

    String toString();

protected:
    std::map <String, String> environment_;
};

#endif //ENVIRONMENTBUILDER_HPP