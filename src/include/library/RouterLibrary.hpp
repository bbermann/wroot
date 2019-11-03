#pragma once

#include "CustomLibrary.hpp"
#include <optional>
#include <nlohmann/json.hpp>

class RouterLibrary : public CustomLibrary {
public:
    RouterLibrary() : CustomLibrary() {}

    ~RouterLibrary() override = default;

    Response handle(const Request &request) override;
};
