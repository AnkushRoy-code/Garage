#ifndef INCLUDE_SRC_PARTICLE_H_
#define INCLUDE_SRC_PARTICLE_H_

#include "Core/Context.h"

struct ParticleDataSend
{
    glm::vec2 Position {};
    float Radius {};
    float _padding {};
    glm::vec4 Color {};
};

struct ParticleData
{
    glm::vec2 Position {};
    glm::vec2 Velocity {};
    glm::vec2 Acceleration {};
    float Mass {};
    float Radius {};
    glm::vec4 Color {};

    bool operator==(const ParticleData &other) const;
};

struct PredefinedParticlesData
{
    const int Count;
    std::vector<ParticleData> Particles;
};

struct ParticleContainer
{
    std::vector<ParticleData> ParticleVec;
    static unsigned int count;
    static const float G;

    void Init();
    void Update();
    void Quit();
    void InitData(unsigned int index);
    std::vector<PredefinedParticlesData> ParticlesData;
};

#endif  // INCLUDE_SRC_PARTICLE_H_
