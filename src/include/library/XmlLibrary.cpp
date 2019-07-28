#include "../core/Core.hpp"
#include "XmlLibrary.hpp"

XmlLibrary::XmlLibrary() : CustomLibrary() {
    this->responseType = "application/xhtml";
}

XmlLibrary::~XmlLibrary() {
}

void XmlLibrary::HttpError(Xml &xml, int error_code) {
    XmlCode response("HttpError"), response_code("Code"), response_message("Message");

    String tmp = to_string(error_code);
    response_code.addChildTag(tmp);

    switch (error_code) {
        case 204:
            response_message.addChildTag("No Content");
            break;

        case 404:
            response_message.addChildTag("Not Found");
            break;
    }

    response.addChildTag(response_code.ToString());
    response.addChildTag(response_message.ToString());
    xml.setElementString(response);
}

String XmlLibrary::toString() {
    return "";
}