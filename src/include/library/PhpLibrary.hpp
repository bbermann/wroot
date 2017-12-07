#ifndef PHPLIBRARY_H
#define PHPLIBRARY_H

#define BUFFER_SIZE 1024

#include "CustomLibrary.hpp"

class PhpLibrary : public CustomLibrary
{
public:
    PhpLibrary();
    virtual ~PhpLibrary();
    virtual String toString() override;

protected:
	String getFullPath();
    String getEnvironment();
};

#endif // PHPLIBRARY_H
