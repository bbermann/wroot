#pragma once

#include <include/library/CustomLibrary.hpp>
#include <include/network/http/Request.hpp>

class HttpSample : public CustomLibrary {
public:
    HttpSample(const Request &request);

    virtual ~HttpSample();

    void handle(Response &response) override;
};
