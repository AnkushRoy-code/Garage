#ifndef INCLUDE_UTILS_TIMER_H_
#define INCLUDE_UTILS_TIMER_H_

#include <chrono>

class Timer  {
public:
    explicit Timer (float &val);
    ~Timer ();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    float &m_TotalTime;
    
};

#endif  // INCLUDE_UTILS_TIMER_H_
