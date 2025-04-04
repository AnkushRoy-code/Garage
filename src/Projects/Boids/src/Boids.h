#ifndef INCLUDE_SRC_BOIDS_H_
#define INCLUDE_SRC_BOIDS_H_

#include "Core/Common/pch.h"

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
    glm::vec4 Color  = {0.455f, 0.780f, 0.925f, 0.5f};
    float seperation = 0.05f;
    float alignment  = 0.05f;
    float cohesion   = 0.0005f;

  private:
    std::vector<BoidsEntity> BoidsVec {};
    const unsigned int NUM_BOIDS = 100;  // changing this requires building the transfer buffers. So
                                         // I refrained from doing that.
};

#endif  // INCLUDE_SRC_BOIDS_H_
