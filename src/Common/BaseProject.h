#ifndef INCLUDE_COMMON_BASEPROJECT_H_
#define INCLUDE_COMMON_BASEPROJECT_H_

#include <memory>
#include <string>
#include <vector>

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

    [[nodiscard]] virtual const std::string &getName() = 0;
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
    static std::vector<std::unique_ptr<BaseProject>> getProjects();

    /// @brief registers all the projects to the vector, should be called before calling
    /// getProjects()
    static void registerAllProjects();  // defined in RegisgerProjects.cpp

    /// @brief register a project to the vector, should be called before calling getProjects()
    static void registerProject(std::unique_ptr<BaseProject> project);

  private:
    static std::vector<std::unique_ptr<BaseProject>>
        projects;  ///< A vector that holds the projects
};

extern std::vector<std::unique_ptr<BaseProject>> Projects;

#endif  // INCLUDE_COMMON_BASEPROJECT_H_
