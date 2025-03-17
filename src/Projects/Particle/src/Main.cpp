// There has been an effort to make this whole project in C style.
#include "Main.h"
#include "Core/Context.h"
#include "Projects/Particle/src/ImGuiMenu.h"

bool Particle::Init()
{
    Name = "Particle";
    return true;
}
bool Particle::Update()
{
    ImGuiParticleWindow();
    return true;
}
bool Particle::Draw()
{
    return true;
}
void Particle::Quit() {}
