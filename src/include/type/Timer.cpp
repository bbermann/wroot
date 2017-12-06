#include "Timer.hpp"

#ifdef WINDOWS
#include <Windows.h>
#include <io.h>
#else
#include "unistd.h"
#include "time.h"
#endif

Timer::Timer() {
}

Timer::~Timer() {
}

#ifndef WINDOWS

unsigned long Timer::GetTickCountLinux() {
    struct timespec ts;
    unsigned theTick = 0U;
    clock_gettime(CLOCK_REALTIME, &ts);
    theTick = ts.tv_nsec / 1000; //divided by 1000 = micro, 100000 = mili
    theTick += ts.tv_sec * 1000;
    return theTick;
}
#endif

unsigned long Timer::now() {
#ifdef WINDOWS
    return GetTickCount();
#else
    return GetTickCountLinux();
#endif    
}

unsigned long Timer::difference(unsigned long start_time, unsigned long end_time) {
    return end_time - start_time;
}

void Timer::start() {
    start_ = Timer::now();
}

unsigned long Timer::finish() {
#ifdef WINDOWS
    finish_ = GetTickCount();
#else
    finish_ = GetTickCountLinux();
#endif

    diference_ = difference(start_, finish_);
    return diference_;
}

unsigned long Timer::last() {
    return diference_;
}
