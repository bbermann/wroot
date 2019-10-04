#pragma once

#include "CustomLibrary.hpp"
#include <include/core/HttpResponse.hpp>
#include <include/helper/LuaScript.hpp>

class RouterLibrary : public CustomLibrary {
public:
    explicit RouterLibrary(const HttpRequest &request);

    virtual ~RouterLibrary();

    String toString() override;

private:
    void registerRequest(LuaScript &routerPlugin);
    void registerResponse(LuaScript &routerPlugin);
};
