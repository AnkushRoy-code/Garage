#include "Common/BaseProject.h"
#include "Projects/SimpleExample/SimpleExample.h"
#include "Projects/Triangle/Triangle.h"
#include "Projects/Particle/src/Main.h"
#include <memory>

void ProjectManager::registerAllProjects()
{
    ProjectManager::registerProject(std::make_unique<SimpleExample>());
    ProjectManager::registerProject(std::make_unique<Triangle>());
    ProjectManager::registerProject(std::make_unique<Particle>());
}
