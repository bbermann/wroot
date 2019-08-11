//
// Created by Bruno dos Santos Bermann on 29/07/2019.
//

#pragma once

#include <exception>

class KeyNotFound : public std::exception {
public:
    explicit KeyNotFound(const String &message): std::exception(), errorMessage(message) {

    }

    virtual char const *what() const noexcept override {
        return this->errorMessage.c_str();
    }

private:
    String errorMessage;
};
