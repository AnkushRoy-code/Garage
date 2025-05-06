#include <atomic>
#include <thread>

class Garage
{
  public:
    static void InitCoreSystems();
    static void ChangeProject();
    static void InitiateProjectUpdateLoop();
    static void RunProjectUpdateLoop();
    static void StopProjectUpdateLoop();

  public:
    static std::atomic<bool> RenderingDone;
    static std::thread updateProject;
};
