#include "ParticleContainer.h"
#include "Core/Context.h"
#include "Projects/Particle/src/ParticleData.h"
#include "Utils/Time.h"
#include <cmath>
#include <cstddef>

ParticleContainer gParticles {};

ParticleContainer::ParticleContainer()
{
    mPositions_X.reserve(8000);  // 8k it the maximum amount of particles possible
    mPositions_Y.reserve(8000);
    mVelocity_X.reserve(8000);
    mVelocity_Y.reserve(8000);
    mColors.reserve(8000);
}

void ParticleContainer::update()
{
    float halfWidth  = 0.5f * gContext.width;
    float halfHeight = 0.5f * gContext.height;

    const float dampingFactor = 0.95f;

    for (size_t i = 0; i < mPositions_Y.size(); i++)
    {
        float xVelocity = mVelocity_X[i];
        float yVelocity = mVelocity_Y[i];

        for (size_t j = 0; j < mPositions_Y.size(); j++)
        {
            if (i == j) continue;  // Skip self-interaction

            float dx = mPositions_X[j] - mPositions_X[i];
            float dy = mPositions_Y[j] - mPositions_Y[i];

            // Wrap around particle force calculation
            if (dx > halfWidth) { dx -= gContext.width; }
            else if (dx < -halfWidth) { dx += gContext.width; }

            if (dy > halfHeight) { dy -= gContext.height; }
            else if (dy < -halfHeight) { dy += gContext.height; }

            // Calculate distance
            float distanceSquared = dx * dx + dy * dy;

            if (distanceSquared == 0.0f) continue;  // Skip if particles overlap

            int colorI = mColors[i];
            int colorJ = mColors[j];

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

        mPositions_X[i] += xVelocity * Utils::Time::deltaTime();
        mPositions_Y[i] += yVelocity * Utils::Time::deltaTime();

        mVelocity_X[i] = xVelocity * dampingFactor;
        mVelocity_Y[i] = yVelocity * dampingFactor;
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
