#ifndef INCLUDE_SRC_MAIN_H_
#define INCLUDE_SRC_MAIN_H_

#include "Common/BaseProject.h"
#include "SDL3/SDL_gpu.h"

typedef struct SpriteInstance
{
    float x, y, z;
    float rotation;
    float w, h, padding_a, padding_b;
    float tex_u, tex_v, tex_w, tex_h;
    float r, g, b, a;
} SpriteInstance;

typedef struct Matrix4x4
{
    float m11, m12, m13, m14;
    float m21, m22, m23, m24;
    float m31, m32, m33, m34;
    float m41, m42, m43, m44;
} Matrix4x4;

Matrix4x4 Matrix4x4_CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane);


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

    SDL_GPUGraphicsPipeline *RenderPipeline;
    SDL_GPUSampler *Sampler;
    SDL_GPUTexture *Texture;
    SDL_GPUTransferBuffer *SpriteDataTransferBuffer;
    SDL_GPUBuffer *SpriteDataBuffer;
};

#endif  // INCLUDE_SRC_MAIN_H_
