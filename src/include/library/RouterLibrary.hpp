#pragma once

#define BUFFER_SIZE 1024

#include "CustomLibrary.hpp"
#include <include/core/HttpResponse.hpp>

class RouterLibrary : public CustomLibrary {
public:
    explicit RouterLibrary(const HttpRequest &request);

    virtual ~RouterLibrary();

    virtual String toString() override;
};
