#pragma once

#include "../core/Core.hpp"
#include "include/network/http/Response.hpp"
#include "include/network/http/Request.hpp"

class CustomLibrary {
public:
    CustomLibrary() = default;
    virtual ~CustomLibrary() = default;

    virtual Response handle(const Request &request) = 0;

protected:
    /// Perform URL-decoding on a string. Returns false if the encoding was invalid.
    static bool urlDecode(const std::string &in, std::string &out);
};
