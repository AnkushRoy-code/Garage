#ifndef INCLUDE_COMMON_BASEPROJECT_H_
#define INCLUDE_COMMON_BASEPROJECT_H_

#include "Common/Context.h"
#include <algorithm>
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

    virtual bool Init(Context &context)   = 0;
    virtual bool Update(Context &context) = 0;
    virtual bool Draw(Context &context)   = 0;
    virtual void Quit(Context &context)   = 0;

  protected:
    std::string Name {};
};

class ProjectManager
{
  public:
    static std::vector<std::unique_ptr<BaseProject>> getProjects();
    static void registerAllProjects();  // defined in RegisgerProjects.cpp
    static void registerProject(std::unique_ptr<BaseProject> project);

  private:
    static std::vector<std::unique_ptr<BaseProject>> projects;
};

#endif  // INCLUDE_COMMON_BASEPROJECT_H_
