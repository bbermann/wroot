#pragma once

#include "String.hpp"

class XmlCode {
public:
    XmlCode(String tag, String id = "");

    ~XmlCode();

    void addChildTag(String value);

    String ToString();

private:
    String tag_, value_, id_;
};