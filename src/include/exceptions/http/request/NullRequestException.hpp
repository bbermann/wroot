//
// Created by Bruno dos Santos Bermann on 07/10/2019.
//

#pragma once

#include <exception>

class NullRequestException : public std::exception {
public:
    virtual char const *what() const noexcept override {
        return "Null request isn't valid input.";
    }
};
