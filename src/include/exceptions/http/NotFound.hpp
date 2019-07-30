//
// Created by Bruno dos Santos Bermann on 29/07/2019.
//

#pragma once

#include "../../type/Exception.hpp"

class NotFound : public Exception {
public:
    NotFound() : Exception("Http::NotFound") {}
};
