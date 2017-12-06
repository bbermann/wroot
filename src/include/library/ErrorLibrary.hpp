#pragma once
#include "CustomLibrary.hpp"

class ErrorLibrary : public CustomLibrary
{
public:
    ErrorLibrary();
    virtual ~ErrorLibrary();

    virtual void root(std::string& plain_text);
	virtual String toString() override;
};

