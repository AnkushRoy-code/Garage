#ifndef INCLUDE_SRC_MAIN_H_
#define INCLUDE_SRC_MAIN_H_

#include "Projects/Common/BaseProject.h"
#include "Projects/Boids/src/Boids.h"
#include "Core/Common/pch.h"

struct BoidsDataSend
{
    float x, y, z;
    float rotation;
    float r, g, b, a;
};

struct RGB_BoidDataSend
{
    float x, y;
    float time;
    float rotation;
};

class Boids : public Common::BaseProject, public Common::ImGuiUI
{
    bool Init() override;
    bool Update() override;
    bool Draw() override;
    void Quit() override;

    std::string name = "Boids";
    [[nodiscard]] const std::string &getName() override
    {
        return name;
    }

    bool DrawUI() override;

    SDL_GPUGraphicsPipeline *renderPipeline;
    SDL_GPUTransferBuffer *boidsDataTransferBuffer;
    SDL_GPUBuffer *boidsDataBuffer;

    bool rgb = false; // doesn't work
    SDL_GPUGraphicsPipeline *rgbRenderPipeline;
    SDL_GPUTransferBuffer *rgbBoidsDataTransferBuffer;
    SDL_GPUBuffer *rgbBoidsDataBuffer;

    BoidsContainer boidsContainer {};
};

#endif  // INCLUDE_SRC_MAIN_H_
