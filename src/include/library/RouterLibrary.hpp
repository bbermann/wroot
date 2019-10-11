#pragma once

#include "CustomLibrary.hpp"
#include <include/network/http/Response.hpp>
#include <include/helper/LuaScript.hpp>

class RouterLibrary : public CustomLibrary {
public:
    explicit RouterLibrary(const Request &request);

    virtual ~RouterLibrary();

    String toString() override;

private:
    void registerRequest(LuaScript &routerPlugin);
    void registerResponse(LuaScript &routerPlugin);
};
