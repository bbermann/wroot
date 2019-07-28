#ifndef HTMLELEMENT_HPP
#define HTMLELEMENT_HPP

#include "String.hpp"

class HtmlElement;

class HtmlElement {
public:
    HtmlElement(std::string tag = "", std::string unique_id = "");

    ~HtmlElement();

    void addChild(HtmlElement element);

    void clear();

    void useTemplate(std::string id);

    void setParameter(std::string parameter, std::string value);

    void removeChild(std::string unique_id);

    void replaceChild(std::string current_id, HtmlElement &replace);

    void setContent(std::string content, bool prependContent = false);

    void setTag(std::string tag);

    void setId(std::string unique_id);

    std::string toString(int level = 0);

    std::string getId();

    static std::vector <HtmlElement> templates;

    static void addTemplate(HtmlElement element);

    static void createTemplates();

protected:
    void _addParameter(std::string parameter, std::string value);

    std::vector <HtmlElement> elements_;
    StringList parameters_;
    std::string content_, tag_, id_;
    bool prependContent_ = false;
    HtmlElement *parent_;
};

#endif
