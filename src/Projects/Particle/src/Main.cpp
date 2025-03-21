#include "Main.h"

#include "Projects/Particle/src/ImGuiMenu.h"
#include "Projects/Particle/src/ParticleContainer.h"
#include "Projects/Particle/src/ParticleController.h"
#include "Projects/Particle/src/ParticleData.h"

#include "Common/Common.h"

bool Particle::Init()
{
    hasUI = true;
    ParticleController::defaultForce();
    ParticleController::defaultMinDistance();
    ParticleController::defaultMaxDistance();
    ParticleController::initializeParticle(gParticleData.ParticleCount,
                                           gParticleData.NumOfParticleColor);
    return true;
}
bool Particle::Update()
{
    PROFILE_SCOPE_N("Particle Update");
    gParticles.update();
    return true;
}
bool Particle::Draw()
{
    return true;
}
void Particle::Quit() {}

bool Particle::DrawUI()
{
    ImGuiParticleWindow();
    return true;
}
