#pragma once
#include "../include/library/HtmlLibrary.hpp"

class HttpSample :
public HtmlLibrary
{
public:
    HttpSample();
    virtual ~HttpSample();

    virtual String toString() override;
};
