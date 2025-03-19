#include "ParticleController.h"
#include "Projects/Particle/src/ParticleData.h"
#include "Projects/Particle/src/ParticleDefaultData.h"
#include "Projects/Particle/src/ParticleContainer.h"

void ParticleController::initializeParticle(unsigned int ParticleCount,
                                            unsigned int NumOfParticleColor)
{
    gParticles.clear();

    for (int color = 0; color < NumOfParticleColor; color++)
    {
        for (int m = 0; m < ParticleCount; m++)
        {
            auto x = disPositionX(gen);
            auto y = disPositionY(gen);
            gParticles.addParticle(x, y, color);
        }
    }
}

void ParticleController::changeAllMin(int value)
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MinDist[(i * COLOR_COUNT) + j] = value;
        }
    }
}

void ParticleController::changeAllMax(int value)
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MaxDist[(i * COLOR_COUNT) + j] = value;
        }
    }
}

void ParticleController::defaultForce()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.Force[(i * COLOR_COUNT) + j] = ParticleDefaultData::m_defaultForceValue[(i * COLOR_COUNT) + j];
        }
    }
}

// TODO
void ParticleController::defaultMinDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MinDist[(i * COLOR_COUNT) + j] =
                ParticleDefaultData::m_defaultMinDistanceValue[(i * COLOR_COUNT) + j];
        }
    }
}

// TODO
void ParticleController::defaultMaxDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MaxDist[(i * COLOR_COUNT) + j] =
                ParticleDefaultData::m_defaultMaxDistanceValue[(i * COLOR_COUNT) + j];
        }
    }
}

void ParticleController::populateRandomForce()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.Force[(i * COLOR_COUNT) + j] = disForce(gen);
        }
    }
}

void ParticleController::populateRandomMinDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MinDist[(i * COLOR_COUNT) + j] = disMinDist(gen);
        }
    }
}

void ParticleController::populateRandomMaxDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MaxDist[(i * COLOR_COUNT) + j] = disMaxDist(gen);
        }
    }
}

void ParticleController::minimiseForce()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.Force[(i * COLOR_COUNT) + j] = minForce;
        }
    }
}

void ParticleController::minimiseMinDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MinDist[(i * COLOR_COUNT) + j] = minMinDist;
        }
    }
}

void ParticleController::minimiseMaxDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MaxDist[(i * COLOR_COUNT) + j] = minMaxDist;
        }
    }
}

void ParticleController::maximiseForce()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.Force[(i * COLOR_COUNT) + j] = maxForce;
        }
    }
}

void ParticleController::maximiseMinDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MinDist[(i * COLOR_COUNT) + j] = maxMinDist;
        }
    }
}

void ParticleController::maximiseMaxDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MaxDist[(i * COLOR_COUNT) + j] = maxMaxDist;
        }
    }
}

void ParticleController::resetForce()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.Force[(i * COLOR_COUNT) + j] = 0;
        }
    }
}
