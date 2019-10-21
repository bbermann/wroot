#pragma once

#include "../core/Core.hpp"
#include "include/network/http/Response.hpp"
#include "include/network/http/Request.hpp"

class CustomLibrary {
public:
    explicit CustomLibrary(const Request &request);
    virtual ~CustomLibrary();

    virtual void handle(Response &response) = 0;

    const Request& getHttpRequest();

protected:
    const Request &request;

    /// Perform URL-decoding on a string. Returns false if the encoding was invalid.
    static bool urlDecode(const std::string &in, std::string &out);
};
