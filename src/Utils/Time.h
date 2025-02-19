#ifndef INCLUDE_UTILS_DELTATIME_H_
#define INCLUDE_UTILS_DELTATIME_H_

#include <chrono>

namespace Utils
{

class Time
{
    // --------------------- DeltaTime ---------------------
  public:
    static void updateDeltaTime() noexcept;
    static void init() noexcept;

    /// Delta time or delta timing is the time elapsed between the previous
    /// frame and the current frame in a game loop.
    static double deltaTime() noexcept;

    /// Get the time in milliseconds since the start of the program (like
    /// SDL_GetTicks)
    static uint32_t getTicks() noexcept;

  private:
    static double mDeltaTime;

    static std::chrono::time_point<std::chrono::high_resolution_clock> mPreviousTime;
    static std::chrono::time_point<std::chrono::high_resolution_clock> mCurrentTime;
    static std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;

    // --------------------- Cap FPS ---------------------
  public:
    static void capFPS();

  private:
    static const int mTargetFPS;
    static const std::chrono::milliseconds mFrameDuration;  // Frame duration
};

}  // namespace Utils

#endif  // INCLUDE_UTILS_DELTATIME_H_
