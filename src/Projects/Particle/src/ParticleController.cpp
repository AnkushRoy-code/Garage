#include "ParticleController.h"
#include "Projects/Particle/src/Particle.h"

// TODO
void ParticleController::initializeParticle(unsigned int ParticleCount,
                                            unsigned int NumOfParticleColor)
{
    // m_particles.clear();

    for (int color = 0; color < NumOfParticleColor; color++)
    {
        for (int m = 0; m < ParticleCount; m++)
        {
            auto x = disPositionX(gen);
            auto y = disPositionY(gen);
            // m_particles.addParticle(x, y, color);
        }
    }
}

void ParticleController::changeAllMin(int value)
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MinDist[i][j] = value;
        }
    }
}

void ParticleController::changeAllMax(int value)
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MaxDist[i][j] = value;
        }
    }
}

// TODO
void ParticleController::defaultForce()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            // gParticleData.Force[i][j] = m_defaultForceValue[i][j];
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
            // gParticleData.MinDist[i][j] = m_defaultForceValue[i][j];
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
            // gParticleData.MaxDist[i][j] = m_defaultForceValue[i][j];
        }
    }
}

void ParticleController::populateRandomForce()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.Force[i][j] = disForce(gen);
        }
    }
}

void ParticleController::populateRandomMinDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MinDist[i][j] = disMinDist(gen);
        }
    }
}

void ParticleController::populateRandomMaxDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MaxDist[i][j] = disMaxDist(gen);
        }
    }
}

void ParticleController::minimiseForce()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.Force[i][j] = minForce;
        }
    }
}

void ParticleController::minimiseMinDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MinDist[i][j] = minMinDist;
        }
    }
}

void ParticleController::minimiseMaxDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MaxDist[i][j] = minMaxDist;
        }
    }
}

void ParticleController::maximiseForce()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.Force[i][j] = maxForce;
        }
    }
}

void ParticleController::maximiseMinDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MinDist[i][j] = maxMinDist;
        }
    }
}

void ParticleController::maximiseMaxDistance()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.MaxDist[i][j] = maxMaxDist;
        }
    }
}

void ParticleController::resetForce()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            gParticleData.Force[i][j] = 0;
        }
    }
}
