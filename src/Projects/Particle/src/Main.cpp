// There has been an effort to make this whole project in C style.
#include "Main.h"
#include "Core/Context.h"
#include "Projects/Particle/src/ImGuiMenu.h"

bool Particle::Init()
{
    Name  = "Particle";
    hasUI = true;
    return true;
}
bool Particle::Update()
{
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
