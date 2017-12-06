#pragma once
#include "CustomLibrary.hpp"
#include "../type/Xml.hpp"

class XmlLibrary : public CustomLibrary
{
public:
    XmlLibrary();
    virtual ~XmlLibrary();

    virtual void root(Xml& xml) = 0;
	virtual String toString() override;

    void HttpError(Xml& xml, int error_code);
};