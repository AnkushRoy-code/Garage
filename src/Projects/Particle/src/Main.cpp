// There has been an effort to make this whole project in C style.
#include "Main.h"
#include "Common/Context.h"
#include "Projects/Particle/src/ImGuiMenu.h"

bool Particle::Init(Context &context)
{
    Name = "Particle";
    return true;
}
bool Particle::Update(Context &context)
{
    ImGuiParticleWindow();
    return true;
}
bool Particle::Draw(Context &context)
{
    return true;
}
void Particle::Quit(Context &context) {}
