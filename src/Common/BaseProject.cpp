#include "Common/BaseProject.h"

std::string BaseProject::Name {};

std::vector<std::unique_ptr<BaseProject>> ProjectManager::projects {};
std::vector<std::unique_ptr<BaseProject>> ProjectManager::getProjects()
{
    return std::move(projects);
}

void ProjectManager::registerProject(std::unique_ptr<BaseProject> project)
{
    projects.emplace_back(std::move(project));
}
