#include "Boids.h"
#include "Core/Context.h"
#include "glm/trigonometric.hpp"
#include <random>

BoidsContainer::BoidsContainer()
{
    BoidsVec.reserve(NUM_BOIDS);
}

void BoidsContainer::init()
{
    const float w = gContext.appState.ProjectWindowX;
    const float h = gContext.appState.ProjectWindowY;

    for (unsigned int i = 0; i < NUM_BOIDS; ++i)
    {
        BoidsEntity boid {};
        boid.Position = glm::vec2(w / 2, h / 2);
        boid.Velocity = glm::vec2(0.0f, 0.0f);
        boid.Rotation = 0.0f;
        BoidsVec.push_back(boid);
    }
}
void BoidsContainer::changeData(float Seperation, float Alignment, float Cohesion)
{
    seperation = Seperation;
    alignment  = Alignment;
    cohesion   = Cohesion;
}

void BoidsContainer::update()
{
    for (auto &boid: BoidsVec)
    {
        // update rotation
        if (boid.Velocity.x == 0.0f && boid.Velocity.y == 0.0f) { boid.Rotation += 0.01; }
        else { boid.Rotation = glm::atan(boid.Velocity.y, boid.Velocity.x); }
    }
}

const std::vector<BoidsEntity> &BoidsContainer::getBoids() const
{
    return BoidsVec;
}
