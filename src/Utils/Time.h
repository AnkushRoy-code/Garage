#ifndef INCLUDE_UTILS_DELTATIME_H_
#define INCLUDE_UTILS_DELTATIME_H_

#include <chrono>

namespace Utils
{

/**
 * @brief Utility class for handling delta time and FPS capping.
 *
 * This class provides static methods to manage delta timing, which measures
 * the time elapsed between frames, and functions for retrieving program ticks.
 */
class Time
{
    // --------------------- DeltaTime ---------------------
  public:
    /**
     * @brief Updates the delta time value.
     *
     * This method should be called once at the start of every frame to calculate the time
     * difference between the current and previous frame.
     */
    static void UpdateDeltaTime() noexcept;

    /**
     * @brief Initializes the time tracking system.
     *
     * This method sets the start time and initializes the timing variables.
     * This function should be called only once during initialisation.
     */
    static void Init() noexcept;

    /**
     * @brief Retrieves the time elapsed between the last two frames.
     *
     * @return The delta time in milliseconds.
     */
    static double DeltaTime() noexcept;

    /**
     * @brief Gets the time in milliseconds since the start of the program.
     *
     * @return The number of milliseconds since the program began execution.
     */
    static uint32_t GetTicks() noexcept;

  private:
    static double m_DeltaTime;  ///< Stores the delta time value in seconds.

    static std::chrono::time_point<std::chrono::high_resolution_clock>
        m_PreviousTime;  ///< Stores the previous frame's timestamp.
    static std::chrono::time_point<std::chrono::high_resolution_clock>
        m_CurrentTime;  ///< Stores the current frame's timestamp.
    static std::chrono::time_point<std::chrono::high_resolution_clock>
        m_StartTime;  ///< Stores the program start timestamp.

    // --------------------- Cap FPS ---------------------
  public:
    /**
     * @brief Caps the FPS to the target frame rate.
     *
     * This function ensures that the frame rate does not exceed a predefined limit.
     */
    static void CapFPS();

  private:
    static constexpr int m_TargetFPS = 60;  ///< The target frames per second.
    static constexpr std::chrono::milliseconds m_FrameDuration =
        std::chrono::milliseconds(1000 / m_TargetFPS);  ///< The expected duration of each frame.
};

}  // namespace Utils

#endif  // INCLUDE_UTILS_DELTATIME_H_
