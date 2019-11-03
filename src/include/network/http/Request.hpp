#pragma once

#include <string>
#include <vector>
#include "KeyValuePair.hpp"

/// A request received from a client.
struct Request {
    std::string method;
    std::string uri;
    int httpVersionMajor;
    int httpVersionMinor;
    StringMap headers;
    StringMap body;

    std::string keyBuffer;
    std::string valueBuffer;

    std::string getHeader(const std::string &key) {
        return headers[key];
    }

    std::string getBody(const std::string &key) {
        return body[key];
    }

    std::string getUrl() {
        return String::explode(uri, "?").at(0);
    };

    std::string getQueryString() {
        auto parts = String::explode(uri, "?");

        if (parts.size() > 1) {
            return parts.at(1);
        }

        return "";
    };
};
