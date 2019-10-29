#pragma once

#include "CustomLibrary.hpp"

class EvoLibrary : public CustomLibrary {
public:
    explicit EvoLibrary(const Request &request);

    virtual ~EvoLibrary() = default;

    void handle(Response &response) override;

private:
    std::string createGenome();
};



