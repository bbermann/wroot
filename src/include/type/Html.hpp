#ifndef HTML_HPP
#define HTML_HPP

#include "String.hpp"
#include "HtmlElement.hpp"

using namespace std;

class Html {
public:
    Html(String xml_namespace = "", String manifest = "");

    ~Html();

    void setXmlNamespace(String xml_namespace);

    void setManifest(String manifest);

    HtmlElement head;
    HtmlElement body;

    String getResponse(String htmlCode);

    String formatHtml(String value);

    String toString();

private:
    String xmlns, manifest;
};

#endif // HTML_HPP_INCLUDED
