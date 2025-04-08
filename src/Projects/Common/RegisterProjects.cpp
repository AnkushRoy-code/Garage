#include "Projects/Common/BaseProject.h"

#include "Projects/SimpleExample/SimpleExample.h"
#include "Projects/Triangle/Triangle.h"
#include "Projects/Boids/src/Main.h"
#include "Projects/N_Body_Simulation/src/Main.h"

#include <memory>

void Common::ProjectManager::registerAllProjects()
{
    ProjectManager::registerProject(std::make_unique<SimpleExample>());
    ProjectManager::registerProject(std::make_unique<Triangle>());
    ProjectManager::registerProject(std::make_unique<Boids>());
    ProjectManager::registerProject(std::make_unique<N_Body_Simulation>());
}
