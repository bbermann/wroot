//
// Created by Bruno dos Santos Bermann on 29/07/2019.
//

#pragma once

#include <exception>
#include "../../type/String.hpp"

class LuaScriptException : public std::runtime_error {
public:
    explicit LuaScriptException(const std::string &message)
            : std::runtime_error(message), errorMessage(message) {

    }

    explicit LuaScriptException(const std::string &message, const std::exception &exception)
            : std::runtime_error(message + "\n" + exception.what()), errorMessage(message) {

    }

private:
    std::string errorMessage;
};
