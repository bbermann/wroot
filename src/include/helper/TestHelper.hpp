#ifndef TESTHELPER_HPP
#define TESTHELPER_HPP

#include <stdlib.h>
#include "FileHelper.hpp"

namespace Test {

    static int randomNumber(int min, int max) 
    {
        return rand() % (max - min + 1) + min;
    }
}

#endif // TESTHELPER_HPP