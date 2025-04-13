#include "Projects/Common/BaseProject.h"
#include <memory>

namespace Common
{

bool BaseProject::hasUI = false;

std::shared_ptr<ProjectManager::ProjVec> ProjectManager::instance =
    std::make_shared<ProjectManager::ProjVec>();

std::shared_ptr<ProjectManager::ProjVec> ProjectManager::GetProjects()
{
    return instance;
}

void ProjectManager::RegisterProject(std::unique_ptr<BaseProject> project)
{
    instance->emplace_back(std::move(project));
}

}  // namespace Common
