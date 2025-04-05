#include "CapFPS.h"
#include <thread>

namespace Utils
{

CapZone::CapZone(unsigned int fps) : FPS(fps)
{
    start = std::chrono::high_resolution_clock::now();
}

CapZone::~CapZone()
{
    using namespace std::chrono_literals;

    auto frameEnd = start + std::chrono::milliseconds(1000 / FPS);

    std::this_thread::sleep_until(frameEnd);
}

}  // namespace Utils
