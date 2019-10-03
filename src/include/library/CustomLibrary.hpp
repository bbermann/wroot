#pragma once

#include "../core/Core.hpp"
#include "../core/HttpResponse.hpp"
#include "../core/HttpRequest.hpp"

class CustomLibrary {
public:
    explicit CustomLibrary(const HttpRequest &request);

    virtual ~CustomLibrary();

    const HttpRequest& getHttpRequest();

    virtual String toString();

    HttpResponse getResponse();

protected:
    const HttpRequest &request;
    HttpResponse response;
};
