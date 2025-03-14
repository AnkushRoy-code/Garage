/// This file is used by ImGuiMenu
#ifndef INCLUDE_SRC_PARTICLECONTROLLER_CPP_
#define INCLUDE_SRC_PARTICLECONTROLLER_CPP_

#include <random>

namespace ParticleController
{
void initializeParticle(unsigned int ParticleCount, unsigned int NumOfParticleColor);

void changeAllMin(int value);
void changeAllMax(int value);

// default
void defaultForce();
void defaultMinDistance();
void defaultMaxDistance();

// rand
void populateRandomForce();
void populateRandomMinDistance();
void populateRandomMaxDistance();

// minimise
void minimiseForce();
void minimiseMinDistance();
void minimiseMaxDistance();

// maximise
void maximiseForce();
void maximiseMinDistance();
void maximiseMaxDistance();

/// set force to 0
void resetForce();

static float minForce = -1.0;
static float maxForce = 1.0;
static int minMinDist = 10;
static int maxMinDist = 30;
static int minMaxDist = 150;
static int maxMaxDist = 300;

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<> disPositionX(0, 640);  // change with width and hegith
static std::uniform_real_distribution<> disPositionY(0, 480);
static std::uniform_real_distribution<> disForce(minForce, maxForce);
static std::uniform_real_distribution<> disMinDist(minMinDist, maxMinDist);
static std::uniform_real_distribution<> disMaxDist(minMaxDist, maxMaxDist);

}  // namespace ParticleController

#endif  // INCLUDE_SRC_PARTICLECONTROLLER_CPP_
