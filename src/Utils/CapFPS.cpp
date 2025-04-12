#include "Utils/CapFPS.h"
#include <thread>

namespace Utils
{

CapZone::CapZone(unsigned int fps) : m_FPS(fps)
{
    m_Start = std::chrono::high_resolution_clock::now();
}

CapZone::~CapZone()
{
    using namespace std::chrono_literals;

    auto frameEnd = m_Start + std::chrono::milliseconds(1000 / m_FPS);

    std::this_thread::sleep_until(frameEnd);
}

}  // namespace Utils
