#include "Timer.h"
#include <chrono>

Timer::Timer(float &val) : totalTime(val)
{
    start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
    const auto end                                    = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<float, std::milli> t = end - start;
    totalTime = t.count();
}
