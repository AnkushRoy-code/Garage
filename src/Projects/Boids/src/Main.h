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

    SDL_GPUGraphicsPipeline *RenderPipeline;
    SDL_GPUTransferBuffer *BoidsDataTransferBuffer;
    SDL_GPUBuffer *BoidsDataBuffer;

    bool rgb = false; // doesn't work
    SDL_GPUGraphicsPipeline *RGB_RenderPipeline;
    SDL_GPUTransferBuffer *RGB_BoidsDataTransferBuffer;
    SDL_GPUBuffer *RGB_BoidsDataBuffer;

    BoidsContainerStruct BoidsContainer {};
};

#endif  // INCLUDE_SRC_MAIN_H_
