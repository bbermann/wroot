#pragma once

#include <string>
#include <vector>
#include "Header.hpp"

/// A request received from a client.
struct Request {
    std::string method;
    std::string uri;
    int httpVersionMajor;
    int httpVersionMinor;
    std::vector <Header> headers;
};
