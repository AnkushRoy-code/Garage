#include "Common/BaseProject.h"
#include "Projects/SimpleImGui/SimpleImGui.h"
#include "Projects/SimpleExample/SimpleExample.h"
#include "Projects/Triangle/Triangle.h"
#include <memory>

void ProjectManager::registerAllProjects()
{
    ProjectManager::registerProject(std::make_unique<SimpleExample>());
    ProjectManager::registerProject(std::make_unique<Triangle>());
    ProjectManager::registerProject(std::make_unique<SimpleImGui>());
}
