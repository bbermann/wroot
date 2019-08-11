//
// Created by Bruno dos Santos Bermann on 29/07/2019.
//

#pragma once

#include <exception>

class FailedReceivingData : public std::exception {
public:
    virtual char const *what() const noexcept override {
        return "Failed receiving request data.";
    }
};
