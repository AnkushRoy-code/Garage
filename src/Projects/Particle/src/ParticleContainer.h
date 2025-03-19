#ifndef INCLUDE_SRC_PARTICLECONTAINER_H_
#define INCLUDE_SRC_PARTICLECONTAINER_H_

#include <vector>

class ParticleContainer
{
  public:
    ParticleContainer();

    void update();

    void addParticle(float x, float y, int color);
    void clear();
    void wrapAround();

    std::vector<float> mPositions_X {};
    std::vector<float> mPositions_Y {};
    std::vector<float> mVelocity_X {};
    std::vector<float> mVelocity_Y {};
    std::vector<int> mColors {};
};

extern ParticleContainer gParticles;

#endif  // INCLUDE_SRC_PARTICLECONTAINER_H_
