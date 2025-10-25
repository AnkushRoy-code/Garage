#ifndef INCLUDE_COREIMGUI_COREIMGUI_H_
#define INCLUDE_COREIMGUI_COREIMGUI_H_

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
    static void InitImGuiStyle();
    static void SetImGuiWindowProportions();
    static bool HandleWindowResize();
    static void ShowToolBox();
    static void TB_ProjectSelector();  // ToolBox Functions;
    static void TB_ResolutionSlider();
    static void ShowConsole();
    static void ShowProjectWindow();
    static void ShowProjectRendered();
};
}  // namespace Core

#endif  // INCLUDE_COREIMGUI_COREIMGUI_H_
