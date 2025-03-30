#ifndef INCLUDE_SRC_BOIDS_H_
#define INCLUDE_SRC_BOIDS_H_

#include "glm/fwd.hpp"
#include <SDL3/SDL_pixels.h>
#include <glm/glm.hpp>
#include <vector>

struct BoidsEntity
{
    glm::vec2 Position;
    glm::vec2 Velocity;
    float Rotation;
};

class BoidsContainer
{
  public:
    BoidsContainer();
    void init();
    void changeData(float Seperation, float Alignment, float Cohesion);
    void update();
    // clang-format off
    [[nodiscard]] unsigned int numBoids() const noexcept { return NUM_BOIDS; }
    // clang-format on
    [[nodiscard]] const std::vector<BoidsEntity> &getBoids() const;

  public:
    glm::vec4 Color  = {0.4f, 0.2f, 0.9f, 1.0f};
    float seperation = 10;
    float alignment  = 14;
    float cohesion   = 2;

  private:
    std::vector<BoidsEntity> BoidsVec {};
    const unsigned int NUM_BOIDS = 10;  // changing this requires building the transfer buffers. So
                                        // I refrained from doing that.
};

#endif  // INCLUDE_SRC_BOIDS_H_
