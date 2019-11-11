#pragma once

#include <string>
#include <map>

/// A request received from a client.
struct Request {
    std::string method;
    std::string uri;
    int httpVersionMajor;
    int httpVersionMinor;
    std::multimap<std::string, std::string> headers;
    std::multimap<std::string, std::string> body;

    std::string keyBuffer;
    std::string valueBuffer;

    std::string getHeader(const std::string &key) const {
        auto found = this->headers.find(key);

        if (found == this->headers.end()) {
            return "";
        }

        return found->second;
    }

    std::string getBody(const std::string &key) const {
        auto found = this->body.find(key);

        if (found == this->body.end()) {
            return "";
        }

        return found->second;
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
