#include "HttpSample.hpp"

HttpSample::HttpSample() : HtmlLibrary()
{
}

HttpSample::~HttpSample()
{
}

String HttpSample::toString()
{
	Html html;

    HtmlElement div_body("div", "body");
    html.body.addChild(div_body);
    div_body.clear();

    HtmlElement h1("h1");
    h1.setContent("My Web Page Title!");
    html.body.addChild(h1);
    h1.clear();

    HtmlElement br("br");
    html.body.addChild(br);

    HtmlElement p("p");
    p.setContent("Welcome to wRoot!");
    html.body.addChild(p);
    p.clear();

	return html.toString();
}