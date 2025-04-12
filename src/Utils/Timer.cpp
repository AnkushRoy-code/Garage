#include "Utils/Timer.h"

#include <chrono>

Timer::Timer(float &val) : m_TotalTime(val)
{
    m_Start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
    const auto end                                    = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<float, std::milli> t = end - m_Start;
    m_TotalTime = t.count();
}
