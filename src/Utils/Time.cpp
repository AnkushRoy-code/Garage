#include "Time.h"

#include <chrono>
#include <thread>

namespace Utils
{

double Time::mDeltaTime = 0.0;
std::chrono::time_point<std::chrono::high_resolution_clock> Time::mPreviousTime;
std::chrono::time_point<std::chrono::high_resolution_clock> Time::mCurrentTime;
std::chrono::time_point<std::chrono::high_resolution_clock> Time::mStartTime;

const int Time::mTargetFPS = 60;  // frame/seconds
const std::chrono::milliseconds Time::mFrameDuration =
    std::chrono::milliseconds(1000 / mTargetFPS);  // In miliseconds

void Time::init() noexcept
{
    mStartTime    = std::chrono::high_resolution_clock::now();
    mPreviousTime = mStartTime;
}

uint32_t Time::getTicks() noexcept
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration    = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - mStartTime);
    return static_cast<uint32_t>(duration.count());
}

double Time::deltaTime() noexcept
{
    return mDeltaTime;
}

void Time::updateDeltaTime() noexcept
{
    using namespace std::chrono;

    mCurrentTime                 = high_resolution_clock::now();
    duration<double> elapsedTime = mCurrentTime - mPreviousTime;

    mDeltaTime    = elapsedTime.count() * 1000; // nano -> miliseconds
    mPreviousTime = mCurrentTime;
}

void Time::capFPS()
{
    using namespace std::chrono;

    auto frameTime = high_resolution_clock::now() - mCurrentTime;

    if (frameTime < mFrameDuration)
    {
        std::this_thread::sleep_for(mFrameDuration - frameTime);
    }
}

}  // namespace Utils
