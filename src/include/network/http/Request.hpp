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
    std::vector<KeyValuePair> headers;
    std::vector<KeyValuePair> body;

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
