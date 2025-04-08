#ifndef INCLUDE_SRC_MAIN_H_
#define INCLUDE_SRC_MAIN_H_

#include "Projects/Common/BaseProject.h"
#include "Projects/Boids/src/Boids.h"
#include "Core/Common/pch.h"

typedef struct SpriteInstance
{
    float x, y, z;
    float rotation;
    float r, g, b, a;
} SpriteInstance;

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

    BoidsContainer boidsContainer {};
};

#endif  // INCLUDE_SRC_MAIN_H_
