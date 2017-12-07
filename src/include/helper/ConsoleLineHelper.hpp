#ifndef CONSOLELINEHELPER_HPP
#define CONSOLELINEHELPER_HPP

#include "../type/String.hpp"

class ConsoleLineHelper
{
public:
    ConsoleLineHelper(String command);
    virtual ~ConsoleLineHelper();

    String executeStdOut();

protected:
    String command_;
};

#endif // CONSOLELINEHELPER_HPP