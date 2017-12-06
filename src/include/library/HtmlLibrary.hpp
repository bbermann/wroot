#pragma once
#include "CustomLibrary.hpp"
#include "../type/Html.hpp"

class HtmlLibrary : public CustomLibrary
{
public:
    HtmlLibrary();
    virtual ~HtmlLibrary();

	virtual String toString() override;

    static void httpError(Html& html, int error_code);

protected:
    void useTemplateFile(std::string fileName);

    Html *html;
};
