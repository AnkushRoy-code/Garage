#include "Utils/Time.h"

#include <chrono>
#include <thread>

namespace Utils
{

double Time::m_DeltaTime = 0.0;
std::chrono::time_point<std::chrono::high_resolution_clock> Time::m_PreviousTime;
std::chrono::time_point<std::chrono::high_resolution_clock> Time::m_CurrentTime;
std::chrono::time_point<std::chrono::high_resolution_clock> Time::m_StartTime;

const int Time::m_TargetFPS = 60;  // frame/seconds
const std::chrono::milliseconds Time::m_FrameDuration =
    std::chrono::milliseconds(1000 / m_TargetFPS);  // In miliseconds

void Time::Init() noexcept
{
    m_StartTime    = std::chrono::high_resolution_clock::now();
    m_PreviousTime = m_StartTime;
}

uint32_t Time::GetTicks() noexcept
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_StartTime);

    return static_cast<uint32_t>(duration.count());
}

double Time::DeltaTime() noexcept
{
    return m_DeltaTime;
}

void Time::UpdateDeltaTime() noexcept
{
    using namespace std::chrono;

    m_CurrentTime                 = high_resolution_clock::now();
    duration<double> elapsedTime = m_CurrentTime - m_PreviousTime;

    m_DeltaTime    = elapsedTime.count() * 1000;  // nano -> miliseconds
    m_PreviousTime = m_CurrentTime;
}

void Time::CapFPS()
{
    using namespace std::chrono;

    auto frameTime = m_CurrentTime + m_FrameDuration;

    std::this_thread::sleep_until(frameTime);
}

}  // namespace Utils
