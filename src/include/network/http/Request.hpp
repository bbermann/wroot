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
};
