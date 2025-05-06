#include "Main.h"

#include "Core/Context.h"
#include "Core/Console.h"
#include "Core/Renderer/Renderer.h"
#include "Core/ImGuiCore/FPSTracker.h"

#include "Projects/Common/BaseProject.h"

#include "Utils/CapFPS.h"
#include "Utils/Time.h"
#include "Utils/Timer.h"
#include <thread>

std::atomic<bool> Garage::RenderingDone {false};
std::thread Garage::updateProject;

void Garage::InitCoreSystems()
{
    RenderingDone.store(false);  // double sure

    Core::ConsoleLogBuffer::AddMessage(
        "Welcome to Ankush's Garage\n"
        "This is a Software I made to showcase my Works that I will build over the years."
        "So this is a placeholder for all my stuff! I hope you enjoy what I have made until "
        "now. It might not be much but was HARD");

    Core::Context::GetContext()->init();
    Common::ProjectManager::RegisterAllProjects();
    Common::ProjectManager::GetProjects()
        ->at(0)  // initialise the first proj
        ->Init();
}

void Garage::ChangeProject()
{
    auto &apst = Core::Context::GetContext()->AppState;

    Common::ProjectManager::GetProjects()->at(apst.ProjectIndex)->Quit();
    apst.ProjectIndex = apst.ProjectToBeIndex;

    Common::ProjectManager::GetProjects()->at(apst.ProjectIndex)->Init();
    apst.HasToChangeIndex = false;
}

void Garage::InitiateProjectUpdateLoop()
{
    Garage::RenderingDone.store(false);
    Garage::updateProject = std::thread(&Garage::RunProjectUpdateLoop);
}

void Garage::RunProjectUpdateLoop()
{
    while (!RenderingDone.load())
    {
        Utils::Time::UpdateDeltaTime();
        Utils::CapZone temp(60);

        {
            static float updateTime = 0.0f;
            Tracker::AddUpdateFPSPointQueue(updateTime);
            Timer riesnt(updateTime);

            Common::ProjectManager::GetProjects()
                ->at(Core::Context::GetContext()->AppState.ProjectIndex)
                ->Update();
        }
    }
}
void Garage::StopProjectUpdateLoop()
{
    Garage::RenderingDone.store(true);
    updateProject.join();
    Core::Context::GetContext()->EventHandler.EndFrame();
}
