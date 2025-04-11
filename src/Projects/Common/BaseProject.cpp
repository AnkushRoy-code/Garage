#include "Projects/Common/BaseProject.h"

std::vector<std::unique_ptr<Common::BaseProject>> g_Projects {};

namespace Common
{

bool BaseProject::hasUI = false;

std::vector<std::unique_ptr<BaseProject>> ProjectManager::m_Projects {};

std::vector<std::unique_ptr<BaseProject>> ProjectManager::GetProjects()
{
    return std::move(m_Projects);
}

void ProjectManager::RegisterProject(std::unique_ptr<BaseProject> project)
{
    m_Projects.emplace_back(std::move(project));
}

}  // namespace Common
