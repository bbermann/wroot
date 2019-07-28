#include "../core/Core.hpp"
#include "Xml.hpp"

Xml::Xml() {
}

Xml::~Xml() {
}

void Xml::addElementString(XmlCode xml) {
    element_string_.append(xml.ToString());
}

void Xml::setElementString(XmlCode xml) {
    element_string_ = xml.ToString();
}

String Xml::toString() {
    string strret = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + element_string_;
    return strret;
}

String Xml::getResponse() {
    String tmpstr = this->toString();
    String strret = "";

    StringList strvct = String::regex_search(tmpstr, "<handle>.+");
    if (strvct.size() > 0) {
        strret = strvct.at(0);
        strvct = strret.explode(">");
        if (strvct.size() >= 2) {
            strret = strvct.at(2);
            String::replace(strret, "</handle", "");
        }
    }

    Core::debugLn("RESPONSE: " + strret);

    return strret;
}