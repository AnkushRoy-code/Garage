#include "Projects/Common/BaseProject.h"

#include "Projects/SimpleExample/SimpleExample.h"
#include "Projects/Triangle/Triangle.h"
#include "Projects/Boids/src/Main.h"
#include "Projects/N_Body_Simulation/src/Main.h"
#include "Projects/Cube/Main.h"

#include <memory>

void Common::ProjectManager::RegisterAllProjects()
{
    ProjectManager::RegisterProject(std::make_unique<SimpleExample>());
    ProjectManager::RegisterProject(std::make_unique<Triangle>());
    ProjectManager::RegisterProject(std::make_unique<Boids>());
    ProjectManager::RegisterProject(std::make_unique<N_Body_Simulation>());
    ProjectManager::RegisterProject(std::make_unique<Cube>());
}
