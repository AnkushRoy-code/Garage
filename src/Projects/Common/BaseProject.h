#ifndef INCLUDE_COMMON_BASEPROJECT_H_
#define INCLUDE_COMMON_BASEPROJECT_H_

#include "Core/Common/pch.h"

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
    /// @brief returns the projects vector
    static std::vector<std::unique_ptr<BaseProject>> GetProjects();

    /// @brief registers all the projects to the vector, should be called before calling
    /// getProjects()
    static void RegisterAllProjects();  // defined in RegisgerProjects.cpp

    /// @brief register a project to the vector, should be called before calling getProjects()
    static void RegisterProject(std::unique_ptr<BaseProject> project);

  private:
    static std::vector<std::unique_ptr<BaseProject>>
        m_Projects;  ///< A vector that holds the projects
};

}  // namespace Common

extern std::vector<std::unique_ptr<Common::BaseProject>> g_Projects;

#endif  // INCLUDE_COMMON_BASEPROJECT_H_
