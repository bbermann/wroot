#pragma once

#include "../core/Core.hpp"
#include "include/network/http/Response.hpp"
#include "include/network/http/Request.hpp"

class CustomLibrary {
public:
    explicit CustomLibrary(const Request &request);

    virtual ~CustomLibrary();

    const Request& getHttpRequest();

    virtual String toString();

    Response getResponse();

protected:
    const Request &request;
    Response response;
};
