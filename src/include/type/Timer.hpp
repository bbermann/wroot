#pragma once

class Timer {
public:
    Timer();
    ~Timer();

#ifndef WINDOWS
    static unsigned long GetTickCountLinux();
#endif
    
    static unsigned long difference(unsigned long start_time, unsigned long end_time);
    static unsigned long now();
    
    void start();
    unsigned long finish();
    unsigned long last();

protected:
    unsigned long start_, finish_;
    unsigned long diference_;
};