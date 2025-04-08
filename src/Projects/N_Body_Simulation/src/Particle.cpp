#include "Particle.h"

#include "Utils/Time.h"
#include "Core/Context.h"

unsigned int ParticleContainer::count = 4;
const float ParticleContainer::G      = 500.0f;

void ParticleContainer::init()
{
    const float w = gContext.appState.ProjectWindowX;
    const float h = gContext.appState.ProjectWindowY;

    const ParticleData d1p1 {
        .Position     = {w / 2 - 100.0f, h / 2},
        .Velocity     = {0.0f, 5.0f},
        .Acceleration = {0.0f, 0.0f},
        .Mass         = 10.0f,
        .Radius       = 20.0f,
        .Color        = {0.2f, 0.5f, 0.5f, 1.0f},
    };

    const ParticleData d1p2 {
        .Position     = {w / 2 + 100.0f, h / 2},
        .Velocity     = {0.0f, -5.0f},
        .Acceleration = {0.0f, 0.0f},
        .Mass         = 10.0f,
        .Radius       = 20.0f,
        .Color        = {0.2f, 0.5f, 0.5f, 1.0f},
    };

    const PredefinedParticlesData data1 = {.count = 2, .Particles = {d1p1, d1p2}};

    auto d2p1     = d1p1;
    d2p1.Position = {w / 2 - 100.0f, h / 2};
    d2p1.Velocity = {0.0f, 7.5f};

    auto d2p2     = d1p1;
    d2p2.Position = {w / 2 + 100.0f, h / 2};
    d2p2.Velocity = {0.0f, -7.5f};

    auto d2p3     = d1p1;
    d2p3.Position = {w / 2, h / 2 - 100.0f};
    d2p3.Velocity = {-7.5f, 0.0f};

    auto d2p4     = d1p1;
    d2p4.Position = {w / 2, h / 2 + 100.0f};
    d2p4.Velocity = {7.5f, 0.0f};

    const PredefinedParticlesData data2 = {.count = 4, .Particles = {d2p1, d2p2, d2p3, d2p4}};

    ParticlesData.push_back(data1);
    ParticlesData.push_back(data2);

    initData(data1);
}

void ParticleContainer::initData(const PredefinedParticlesData &data)
{
    count = data.count;
    ParticleVec.clear();
    ParticleVec.reserve(count);

    for (const auto &particle: data.Particles)
    {
        ParticleVec.push_back(particle);
    }
}

void ParticleContainer::update()
{
    for (unsigned int i = 0; i < count; i++)
    {
        // the program sometimes segfaults here. and sometimes doesn't
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

            const float distance = glm::sqrt(dx * dx + dy * dy);

            if (distance < 0.01f) { continue; }

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
