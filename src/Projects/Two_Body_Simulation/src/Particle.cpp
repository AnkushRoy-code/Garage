#include "Particle.h"

#include "Utils/Time.h"
#include "Core/Context.h"

bool ParticleData::operator==(const ParticleData &other) const
{
    // clang-format off
        if (this->Position == other.Position 
            && this->Color == other.Color
            && this->Mass == other.Mass 
            && this->Radius == other.Radius
            && this->Velocity == other.Velocity)  // clang-format on
    {
        return true;
    }
    return false;
}

void ParticleContainer::init()
{
    ParticleVec.reserve(count);

    const float w = gContext.appState.ProjectWindowX;
    const float h = gContext.appState.ProjectWindowY;

    ParticleData particle {};
    ParticleData particle2 {};

    particle.Position  = {w / 2 - 100.0f, h / 2};
    particle2.Position = {w / 2 + 100.0f, h / 2};

    particle.Velocity  = {0.0f, 5.0f};
    particle2.Velocity = {0.0f, -5.0f};

    particle.Mass  = 10.0f;
    particle2.Mass = 10.0f;

    particle.Radius  = 20.0f;
    particle2.Radius = 20.0f;

    particle.Color  = {0.2f, 0.5f, 0.5f, 1.0f};
    particle2.Color = {0.2f, 0.5f, 0.5f, 1.0f};

    particle.Acceleration  = {0.0f, 0.0f};
    particle2.Acceleration = {0.0f, 0.0f};

    ParticleVec.emplace_back(particle);
    ParticleVec.emplace_back(particle2);
}

void ParticleContainer::update()
{
    /// @note
    /// glm::vec2 a; glm::vec2 b;
    /// a += b * (float) / (float) -> results in segfault for some reason
    ///
    /// Workaround:
    /// a.x += b.x * (float) / (float)
    /// a.y += b.y * (float) / (float)

    for (unsigned int i = 0; i < count; i++)
    {
        ParticleVec[i].Acceleration.x = 0.0f;
        ParticleVec[i].Acceleration.y = 0.0f;
    }

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            if (i == j) continue;

            const float dx = ParticleVec[j].Position.x - ParticleVec[i].Position.x;
            const float dy = ParticleVec[j].Position.y - ParticleVec[i].Position.y;

            const float distance =
                glm::sqrt(dx * dx + dy * dy) + 0.01f;  // sacrificed accuracy for performance

            const float normX = dx / distance;
            const float normY = dy / distance;

            const float force =
                G * ParticleVec[i].Mass * ParticleVec[j].Mass / (distance * distance);

            const float accX = (force / ParticleVec[i].Mass) * normX;
            const float accY = (force / ParticleVec[i].Mass) * normY;

            ParticleVec[i].Acceleration.x += accX;
            ParticleVec[i].Acceleration.y += accY;
        }
    }

    float dt = static_cast<float>(Utils::Time::deltaTime()) / 100.0f;

    for (unsigned int i = 0; i < count; i++)
    {
        ParticleVec[i].Velocity.x += ParticleVec[i].Acceleration.x;
        ParticleVec[i].Velocity.y += ParticleVec[i].Acceleration.y;

        ParticleVec[i].Position.x += ParticleVec[i].Velocity.x * dt;
        ParticleVec[i].Position.y += ParticleVec[i].Velocity.y * dt;
    }
}

void ParticleContainer::quit()
{
    ParticleVec.clear();
}
