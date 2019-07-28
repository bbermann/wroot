#include "HtmlLibrary.hpp"
#include "../core/Core.hpp"
#include <fstream>

using namespace std;

HtmlLibrary::HtmlLibrary() : CustomLibrary() {
    this->responseType = "text/html";
}

HtmlLibrary::~HtmlLibrary() {

}

void HtmlLibrary::httpError(Html &html, int error_code) {
    HtmlElement error("strong", "error_msg"), font("font");
    font.setParameter("color", "red");
    font.setParameter("size", "4em");

    String msg = "The server cannot access that URL.<br/>";
    msg.append("Error code: #");
    msg.append(to_string(error_code));

    font.setContent(msg);
    error.addChild(font);
    html.body.addChild(error);
}

void HtmlLibrary::useTemplateFile(string fileName) {
    String::replace(fileName, "/", Core::PathSeparator);

    ifstream templateFile(Core::ApplicationPath + fileName);
    String currentLine = "", body = "", head = "";
    bool isBody = false, isHead = false;

    if (templateFile.is_open()) {
        while (getline(templateFile, currentLine)) {
            if (currentLine.contains("<head")) {
                isHead = true;
                continue;
            } else if (currentLine.contains("</head>")) {
                isHead = false;
                continue;
            }

            if (currentLine.contains("<body")) {
                isBody = true;
                continue;
            } else if (currentLine.contains("</body>")) {
                isBody = false;
                continue;
            }

            if (isBody) {
                body.append(currentLine);
            }

            if (isHead) {
                head.append(currentLine);
            }
        }

        templateFile.close();
    }

    if (!head.empty()) {
        html->head.setContent(head);
    }

    if (!body.empty()) {
        html->body.setContent(body);
    }
}

String HtmlLibrary::toString() {
    return "";
}