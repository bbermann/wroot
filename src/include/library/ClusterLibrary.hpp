#pragma once
#include "CustomLibrary.hpp"

class ClusterLibrary : public CustomLibrary
{
public:
    ClusterLibrary();
    virtual ~ClusterLibrary();
	virtual String toString() override;
};
