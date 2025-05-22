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
    void Init();
    void Update();
    void Quit();
    [[nodiscard]] unsigned int NumBoids() const noexcept
    {
        return k_NumBoids;
    }
    [[nodiscard]] const std::vector<BoidsEntity> &GetBoids() const;

  public:
    glm::vec4 Color  = {0.455f, 0.780f, 0.925f, 0.5f};
    float Seperation = 0.05f;
    float Alignment  = 0.05f;
    float Cohesion   = 0.0005f;

  private:
    std::vector<BoidsEntity> m_BoidsVec {};
    const unsigned int k_NumBoids = 100;  // changing this requires building the transfer buffers. So
                                          // I refrained from doing that.
};

#endif  // INCLUDE_SRC_BOIDS_H_
