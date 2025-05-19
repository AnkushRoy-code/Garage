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

  private:
    static SDL_GPUTextureSamplerBinding bind;
    static void InitImGuiStyle();
    static void SetImGuiWindowProportions();
    static bool HandleWindowResize();
    static void ShowToolBox();
    static void TB_ProjectSelector(); // ToolBox Functions;
    static void TB_ResolutionSlider();
    static void ShowConsole();
    static void ShowProjectWindow();
    static void ShowProjectRendered();
};
}  // namespace Core

#endif  // INCLUDE_COREIMGUI_COREIMGUI_H_
