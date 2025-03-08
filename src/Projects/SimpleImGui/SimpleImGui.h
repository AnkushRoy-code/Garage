#ifndef INCLUDE_SIMPLEIMGUI_IMGUI_H_
#define INCLUDE_SIMPLEIMGUI_IMGUI_H_

#include "Common/BaseProject.h"

class SimpleImGui : public BaseProject
{
    bool Init(Context &context) override;
    void handleEvent(SDL_Event &event) override;
    bool Update(Context &context) override;
    bool Draw(Context &context) override;
    void Quit(Context &context) override;
};

#endif  // INCLUDE_SIMPLEIMGUI_IMGUI_H_
