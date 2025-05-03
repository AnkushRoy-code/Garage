#ifndef INCLUDE_SRC_MAIN_H_
#define INCLUDE_SRC_MAIN_H_

#include "Core/Common/pch.h"

#include "Projects/Boids/src/Boids.h"
#include "Projects/Common/BaseProject.h"

struct BoidsDataSend
{
    float x, y, z;
    float Rotation;
    float r, g, b, a;
};

struct RGB_BoidDataSend
{
    float x, y;
    float Time;
    float Rotation;
};

class Boids : public Common::BaseProject, public Common::ImGuiUI
{
    bool Init() override;
    bool Update() override;
    bool Draw() override;
    void Quit() override;

    std::string name = "Boids";
    [[nodiscard]] const std::string &GetName() override
    {
        return name;
    }

    bool DrawUI() override;

    SDL_GPUGraphicsPipeline *m_RenderPipeline;
    SDL_GPUTransferBuffer *m_BoidsDataTransferBuffer;
    SDL_GPUBuffer *m_BoidsDataBuffer;

    bool m_RGB = false;
    BoidsContainer m_BoidsContainer {};
};

#endif  // INCLUDE_SRC_MAIN_H_
