#ifndef INCLUDE_COREIMGUI_COREIMGUI_H_
#define INCLUDE_COREIMGUI_COREIMGUI_H_

namespace Core::ImGuiCore
{
void Update();
void Draw();
void Init();
void Quit();
bool HandleWindowResize();
}  // namespace Core::ImGuiCore

#endif  // INCLUDE_COREIMGUI_COREIMGUI_H_
