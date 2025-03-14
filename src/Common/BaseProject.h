#ifndef INCLUDE_COMMON_BASEPROJECT_H_
#define INCLUDE_COMMON_BASEPROJECT_H_

#include "Common/Context.h"
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Structure representing an example application instance.
 *
 * This structure contains function callbacks required for initialization,
 * updating, drawing, and quitting an example.
 */
class BaseProject
{
  public:
    virtual ~BaseProject() = default;

    virtual bool Init(Context &context)        = 0;
    virtual bool Update(Context &context)      = 0;
    virtual bool Draw(Context &context)        = 0;
    virtual void Quit(Context &context)        = 0;

  protected:
    std::string Name {};
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

#endif  // INCLUDE_COMMON_BASEPROJECT_H_
