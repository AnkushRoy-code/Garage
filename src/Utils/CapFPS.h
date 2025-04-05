#ifndef INCLUDE_UTILS_CAPFPS_H_
#define INCLUDE_UTILS_CAPFPS_H_

#include <chrono>
namespace Utils
{
/// This class just caps the function fps with the given constructor argument
class CapZone
{
  public:
    explicit CapZone(unsigned int fps);

    ~CapZone();

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    unsigned int FPS;
};

}  // namespace Utils

#endif  // INCLUDE_UTILS_CAPFPS_H_
