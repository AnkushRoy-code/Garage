#ifndef INCLUDE_COREIMGUI_COREIMGUI_H_
#define INCLUDE_COREIMGUI_COREIMGUI_H_

#include "SDL3/SDL_gpu.h"
namespace Core
{
class ImGuiCore
{
  public:
    static void Update();
    static void Draw();
    static void Init();
    static void Quit();
    static bool HandleWindowResize();

  private:
    static SDL_GPUTextureSamplerBinding bind;
};
}  // namespace Core

#endif  // INCLUDE_COREIMGUI_COREIMGUI_H_
