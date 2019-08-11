#pragma once

#include <include/library/CustomLibrary.hpp>
#include <include/core/HttpRequest.hpp>

class HttpSample : public CustomLibrary {
public:
    HttpSample(const HttpRequest &request);

    virtual ~HttpSample();

    virtual String toString() override;
};
