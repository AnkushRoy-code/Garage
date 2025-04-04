#include "Projects/Common/BaseProject.h"

std::vector<std::unique_ptr<Common::BaseProject>> Projects {};

namespace Common
{

bool BaseProject::hasUI = false;

std::vector<std::unique_ptr<BaseProject>> ProjectManager::projects {};

std::vector<std::unique_ptr<BaseProject>> ProjectManager::getProjects()
{
    return std::move(projects);
}

void ProjectManager::registerProject(std::unique_ptr<BaseProject> project)
{
    projects.emplace_back(std::move(project));
}

}  // namespace Common
