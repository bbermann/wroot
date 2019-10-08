//
// Created by Bruno dos Santos Bermann on 07/10/2019.
//

#pragma once

#include <exception>

class RequestTimeoutException : public std::exception {
public:
    virtual char const *what() const noexcept override {
        return "Request timed out.";
    }
};
