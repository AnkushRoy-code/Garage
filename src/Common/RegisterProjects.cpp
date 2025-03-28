#include "Common/BaseProject.h"
#include "Projects/SimpleExample/SimpleExample.h"
#include "Projects/Triangle/Triangle.h"
#include "Projects/Boids/src/Main.h"
#include <memory>

void Common::ProjectManager::registerAllProjects()
{
    ProjectManager::registerProject(std::make_unique<SimpleExample>());
    ProjectManager::registerProject(std::make_unique<Triangle>());
    ProjectManager::registerProject(std::make_unique<Boids>());
}
