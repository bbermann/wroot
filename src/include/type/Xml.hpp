#ifndef XML_HPP
#define XML_HPP

#include "String.hpp"
#include "XmlCode.hpp"

using namespace std;

class Xml
{
public:
	Xml();
	~Xml();

	void addElementString(XmlCode xml);
	void setElementString(XmlCode xml);
	String toString();
	String getResponse();

private:
	string element_string_;
};

#endif // XML_HPP_INCLUDED