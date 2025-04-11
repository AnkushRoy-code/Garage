#ifndef INCLUDE_CORE_CONTEXT_H_
#define INCLUDE_CORE_CONTEXT_H_

#include "Common/pch.h"

#include "Core/EventHandler.h"

namespace Core
{

struct RenderDataStruct
{
    SDL_Window *Window             = nullptr;
    SDL_GPUDevice *Device          = nullptr;
    SDL_GPURenderPass *ProjectPass = nullptr;
    SDL_GPUTexture *ProjectTexture = nullptr;
    SDL_GPUTexture *ResolveTexture = nullptr;
    SDL_GPUSampler *ProjectSampler = nullptr;
    SDL_GPUSampleCount SampleCount = SDL_GPU_SAMPLECOUNT_1;
    float ResolutionScale          = 1.0f;  // %
    int Width                      = 1280;
    int Height                     = 720;
};

struct AppStateStruct
{
    ImGuiID MainViewportId = 0;
    float ProjectWindowX   = 0;
    float ProjectWindowY   = 0;
    int ProjectIndex       = 0;
    int ProjectToBeIndex   = 0;
    float HorizontalScroll = 0;
    float VerticalScroll   = 0;
    bool HasToChangeIndex  = false;
};

struct Context
{
    Context() = default;
    ~Context();
    void init();

    RenderDataStruct RenderData {};
    AppStateStruct AppState {};
    EventHandlerStruct EventHandler {};
};

}  // namespace Core

extern Core::Context g_Context;
#endif  // INCLUDE_CORE_CONTEXT_H_
