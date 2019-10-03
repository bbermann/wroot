#pragma once

#include "CustomLibrary.hpp"
#include <include/core/HttpResponse.hpp>

class RouterLibrary : public CustomLibrary {
public:
    explicit RouterLibrary(const HttpRequest &request);

    virtual ~RouterLibrary();

    String toString() override;
};
