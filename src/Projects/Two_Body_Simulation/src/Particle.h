#ifndef INCLUDE_SRC_PARTICLE_H_
#define INCLUDE_SRC_PARTICLE_H_

struct ParticleDataSend
{
    glm::vec2 Position;
    float Radius;
    float _padding;
    glm::vec4 Color;
};

struct ParticleData
{
    glm::vec2 Position;
    glm::vec2 Velocity;
    glm::vec2 Acceleration {};
    float Mass;
    float Radius;
    glm::vec4 Color;

    bool operator==(const ParticleData &other) const;
};

struct ParticleContainer
{
    std::vector<ParticleData> ParticleVec;
    unsigned int count = 2;
    const float G      = 500.0f;

    void init();
    void update();
    void quit();
};

#endif  // INCLUDE_SRC_PARTICLE_H_
