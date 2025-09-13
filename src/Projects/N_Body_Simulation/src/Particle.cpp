#include "Projects/N_Body_Simulation/src/Particle.h"

#include "Utils/Time.h"
#include "Core/Context.h"

unsigned int ParticleContainer::count = 2;
const float ParticleContainer::G      = 500.0f;

void ParticleContainer::Init()
{
    const float w = Core::Context::GetContext()->AppState.ProjectWindowSize.x;
    const float h = Core::Context::GetContext()->AppState.ProjectWindowSize.y;

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

    const PredefinedParticlesData data1 = {
        .Count     = 2,
        .Particles = {d1p1, d1p2}
    };

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

    const PredefinedParticlesData data2 = {
        .Count     = 4,
        .Particles = {d2p1, d2p2, d2p3, d2p4}
    };

    const int numCircleParticles = 8;
    ParticleData circleParticles[numCircleParticles];

    const float centerX      = w / 2;
    const float centerY      = h / 2;
    const float circleRadius = 150.0f;  // Radius of bigger circle

    for (int i = 0; i < numCircleParticles; ++i)
    {
        float angle        = (2.0f * SDL_PI_F / numCircleParticles) * i;
        circleParticles[i] = {
            // Position:
            {centerX + circleRadius * std::cos(angle), centerY + circleRadius * std::sin(angle)},
            // Velocity:
            {-10.0f * std::sin(angle), 10.0f * std::cos(angle)},
            // Acceleration:
            {0.0f, 0.0f},
            // Mass:
            5.0f,
            // Radius:
            15.0f,
            // Color: vary the red component slightly
            {0.3f + 0.05f * i, 0.6f, 0.8f, 1.0f}
        };
    }

    const PredefinedParticlesData dataCircular = {
        .Count     = numCircleParticles,
        .Particles = {circleParticles[0], circleParticles[1], circleParticles[2], circleParticles[3], circleParticles[4], circleParticles[5], circleParticles[6], circleParticles[7]}
    };

    const int gridRows = 3;
    const int gridCols = 3;
    ParticleData gridParticles[gridRows * gridCols];
    const float spacing = 75.0f;

    int index = 0;
    for (int r = 0; r < gridRows; ++r)
    {
        for (int c = 0; c < gridCols; ++c)
        {
            float posX           = centerX + (float)c - 1 * spacing;
            float posY           = centerY + (float)r - 1 * spacing;
            gridParticles[index] = {
                // Position:
                .Position = {posX, posY},
                // Velocity: outwards from the center
                .Velocity = {(posX - centerX) * 0.1f, (posY - centerY) * 0.1f},
                // Acceleration:
                .Acceleration = {0.0f, 0.0f},
                // Mass:
                .Mass = 7.0f,
                // Radius:
                .Radius = 20.0f,
                // Color: gradually changing blue component
                .Color = {0.4f, 0.4f, 0.7f - 0.1f * (float)index, 1.0f}
            };
            ++index;
        }
    }

    const PredefinedParticlesData dataGrid = {
        .Count     = gridRows * gridCols,
        .Particles = {gridParticles[0], gridParticles[1], gridParticles[2], gridParticles[3], gridParticles[4], gridParticles[5], gridParticles[6], gridParticles[7], gridParticles[8]}
    };

    ParticlesData.reserve(4);
    ParticlesData.push_back(data1);
    ParticlesData.push_back(data2);
    ParticlesData.push_back(dataCircular);
    ParticlesData.push_back(dataGrid);

    ParticleVec.reserve(16);

    InitData(0);
}

void ParticleContainer::InitData(unsigned int i)
{
    if (ParticlesData.size() < i) { return; }

    count = ParticlesData[i].Count;
    ParticleVec.clear();

    for (const auto &particle: ParticlesData[i].Particles)
    {
        ParticleVec.push_back(particle);
    }
}

void ParticleContainer::Update()
{
    for (auto &i: ParticleVec)
    {
        i.Acceleration = glm::vec2(0.0f);
    }

    for (int i = 0; i < ParticleVec.size(); i++)
    {
        const float radius = ParticleVec[i].Radius;

        for (int j = 0; j < ParticleVec.size(); j++)
        {
            if (i == j) continue;

            const glm::vec2 diff        = ParticleVec[j].Position - ParticleVec[i].Position;
            const float distanceSquared = glm::dot(diff, diff);

            if (distanceSquared < radius * radius) continue;

            const float distance      = glm::sqrt(distanceSquared);
            const glm::vec2 direction = diff / distance;

            const float force            = G * ParticleVec[i].Mass * ParticleVec[j].Mass / distanceSquared;
            const glm::vec2 acceleration = (force / ParticleVec[i].Mass) * direction;

            ParticleVec[i].Acceleration += acceleration;
        }
    }

    const float dt = static_cast<float>(Utils::Time::DeltaTime()) / 100.0f;

    for (auto &i: ParticleVec)
    {
        i.Velocity += i.Acceleration;
        i.Position += i.Velocity * dt;
    }
}

void ParticleContainer::Quit()
{
    ParticleVec.clear();
    ParticlesData.clear();
}

bool ParticleData::operator==(const ParticleData &other) const
{
    return (
        this->Position == other.Position
        && this->Color == other.Color
        && this->Mass == other.Mass
        && this->Radius == other.Radius
        && this->Velocity == other.Velocity);
}
