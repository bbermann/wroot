//
// Created by Bruno dos Santos Bermann on 29/07/2019.
//

#pragma once

#include <exception>

class Unauthorized : public std::exception {
public:
    virtual char const *what() const noexcept override {
        return "Http::Unauthorized";
    }
};
