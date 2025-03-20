#include "ParticleContainer.h"
#include "Core/Context.h"
#include "Projects/Particle/src/ParticleData.h"
#include "Utils/Time.h"

#include <cmath>
#include <cstddef>
#include <thread>

ParticleContainer gParticles {};

ParticleContainer::ParticleContainer()
{
    mPositions_X.reserve(8000);  // 8k it the maximum amount of particles possible
    mPositions_Y.reserve(8000);
    mVelocity_X.reserve(8000);
    mVelocity_Y.reserve(8000);
    mColors.reserve(8000);
}

void updateParticle(int i, float halfWidth, float halfHeight, float &xVelocity, float &yVelocity)
{
    for (size_t j = 0; j < gParticles.mPositions_Y.size(); j++)
    {
        if (i == j) continue;  // Skip self-interaction

        float dx = gParticles.mPositions_X[j] - gParticles.mPositions_X[i];
        float dy = gParticles.mPositions_Y[j] - gParticles.mPositions_Y[i];

        // Wrap around particle force calculation
        if (dx > halfWidth) { dx -= gContext.width; }
        else if (dx < -halfWidth) { dx += gContext.width; }

        if (dy > halfHeight) { dy -= gContext.height; }
        else if (dy < -halfHeight) { dy += gContext.height; }

        // Calculate distance
        float distanceSquared = dx * dx + dy * dy;

        if (distanceSquared == 0.0f) continue;  // Skip if particles overlap

        int colorI = gParticles.mColors[i];
        int colorJ = gParticles.mColors[j];

        // Determine force based on distance
        float force = 0.0f;

        int index = (colorI * COLOR_COUNT) + colorJ;

        float minDistSquared = gParticleData.MinDist[index] * gParticleData.MinDist[index];
        float maxDistSquared = gParticleData.MaxDist[index] * gParticleData.MaxDist[index];

        if (distanceSquared < minDistSquared)
        {
            force          = -1.0f;
            float distance = std::sqrt(distanceSquared);

            float fx = force * (dx / distance);
            float fy = force * (dy / distance);

            xVelocity += fx;
            yVelocity += fy;
        }

        else if (distanceSquared <= maxDistSquared)
        {
            force          = gParticleData.Force[index];
            float distance = std::sqrt(distanceSquared);

            float fx = force * (dx / distance);
            float fy = force * (dy / distance);

            xVelocity += fx;
            yVelocity += fy;
        }
    }
}

void ParticleContainer::update()
{
    float halfWidth  = 0.5f * gContext.width;
    float halfHeight = 0.5f * gContext.height;

    const float dampingFactor = 0.95f;

    size_t numParticles = mPositions_Y.size();
    int numThreads      = std::thread::hardware_concurrency();
    if (numThreads == 0) { numThreads = 2; }

    size_t particlesPerThread = (numParticles + numThreads - 1) / numThreads;
    std::vector<std::thread> threads;
    threads.reserve(numThreads);

    for (unsigned int t = 0; t < numThreads; t++)
    {
        size_t start = t * particlesPerThread;
        size_t end   = std::min(numParticles, start + particlesPerThread);
        threads.emplace_back(
            [this, start, end, halfWidth, halfHeight, dampingFactor]()
            {
                for (size_t i = start; i < end; i++)
                {
                    float xVelocity = mVelocity_X[i];
                    float yVelocity = mVelocity_Y[i];

                    updateParticle(i, halfWidth, halfHeight, xVelocity, yVelocity);

                    mPositions_X[i] += xVelocity * Utils::Time::deltaTime();
                    mPositions_Y[i] += yVelocity * Utils::Time::deltaTime();

                    mVelocity_X[i] = xVelocity * dampingFactor;
                    mVelocity_Y[i] = yVelocity * dampingFactor;
                }
            });
    }

    for (auto &thread: threads)
    {
        thread.join();
    }

    wrapAround();
}

void ParticleContainer::addParticle(float x, float y, int color)
{
    mPositions_X.emplace_back(x);
    mPositions_Y.emplace_back(y);
    mVelocity_X.emplace_back(0.0f);
    mVelocity_Y.emplace_back(0.0f);
    mColors.emplace_back(color);
}

void ParticleContainer::clear()
{
    mPositions_X.clear();
    mPositions_Y.clear();
    mVelocity_X.clear();
    mVelocity_Y.clear();
    mColors.clear();
}

void ParticleContainer::wrapAround()
{
    size_t size = mPositions_X.size();

    for (float &m_xPosition: mPositions_X)
    {
        if (m_xPosition < 0) { m_xPosition += gContext.width; }
        else if (m_xPosition >= gContext.width) { m_xPosition -= gContext.width; }
    }

    for (float &m_yPosition: mPositions_Y)
    {
        if (m_yPosition < 0) { m_yPosition += gContext.height; }
        else if (m_yPosition >= gContext.height) { m_yPosition -= gContext.height; }
    }
}
