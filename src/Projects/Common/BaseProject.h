#ifndef INCLUDE_COMMON_BASEPROJECT_H_
#define INCLUDE_COMMON_BASEPROJECT_H_

#include "Core/Common/pch.h"
#include <memory>

namespace Common
{

/**
 * @brief Structure representing an project application instance.
 *
 * This structure contains function callbacks required for initialization,
 * updating, drawing, and quitting an project.
 */
class BaseProject
{
  public:
    virtual ~BaseProject() = default;

    virtual bool Init()   = 0;
    virtual bool Update() = 0;
    virtual bool Draw()   = 0;
    virtual void Quit()   = 0;

    [[nodiscard]] virtual const std::string &GetName() = 0;
    static bool hasUI;
};

class ImGuiUI
{
  public:
    virtual ~ImGuiUI()    = default;
    virtual bool DrawUI() = 0;
};

class ProjectManager
{
  public:
    ProjectManager();
    ~ProjectManager();

    ProjectManager(ProjectManager &&)                 = delete;
    ProjectManager(const ProjectManager &)            = delete;
    ProjectManager &operator=(ProjectManager &&)      = delete;
    ProjectManager &operator=(const ProjectManager &) = delete;

    using ProjVec = std::vector<std::unique_ptr<BaseProject>>;

    static std::shared_ptr<ProjVec> GetProjects();

    /// @brief
    /// registers all the projects to the vector, should be called before calling
    /// getProjects()
    static void RegisterAllProjects();  // defined in RegisgerProjects.cpp

  private:
    static void RegisterProject(std::unique_ptr<BaseProject> project);
    static std::shared_ptr<ProjVec> instance;
};

}  // namespace Common

#endif  // INCLUDE_COMMON_BASEPROJECT_H_
