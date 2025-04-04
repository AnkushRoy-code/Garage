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

typedef struct Matrix4x4
{
    float m11, m12, m13, m14;
    float m21, m22, m23, m24;
    float m31, m32, m33, m34;
    float m41, m42, m43, m44;
} Matrix4x4;

Matrix4x4 Matrix4x4_CreateOrthographicOffCenter(
    float left, float right, float bottom, float top, float zNearPlane, float zFarPlane);

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
