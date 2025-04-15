#ifndef INCLUDE_CUBE_MAIN_H_
#define INCLUDE_CUBE_MAIN_H_

#include "Projects/Common/BaseProject.h"
#include "Projects/Common/Camera.h"

class Cube : public Common::BaseProject, public Common::ImGuiUI
{
    bool Init() override;
    bool Update() override;
    bool Draw() override;
    void Quit() override;

    bool DrawUI() override;

    std::string name = "Cube";
    [[nodiscard]] const std::string &GetName() override
    {
        return name;
    }

    SDL_GPUGraphicsPipeline *m_RenderPipeline;
    Camera m_Camera ;
};

#endif  // INCLUDE_CUBE_MAIN_H_
