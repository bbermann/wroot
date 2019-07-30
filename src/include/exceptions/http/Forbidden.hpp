//
// Created by Bruno dos Santos Bermann on 29/07/2019.
//

#pragma once

#include "../../type/Exception.hpp"

class Forbidden : public Exception {
public:
    Forbidden() : Exception("Http::Forbidden") {}
};
