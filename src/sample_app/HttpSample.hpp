#pragma once

#include "../include/library/CustomLibrary.hpp"

class HttpSample : public CustomLibrary {
public:
    HttpSample();

    virtual ~HttpSample();

    virtual String toString() override;
};
