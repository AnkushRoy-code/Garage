#include "Common/BaseProject.h"
#include "Examples/SimpleExample/SimpleExample.h"
#include "Examples/Triangle/Triangle.h"
#include <memory>

void ProjectManager::registerAllProjects()
{
    ProjectManager::registerProject(std::make_unique<SimpleExample>());
    ProjectManager::registerProject(std::make_unique<Triangle>());
}
