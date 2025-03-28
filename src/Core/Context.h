#ifndef INCLUDE_CORE_CONTEXT_H_
#define INCLUDE_CORE_CONTEXT_H_

#include "Core/EventHandler.h"
#include <SDL3/SDL.h>
#include <imgui.h>

namespace Core
{

struct RenderData
{
    SDL_Window *window             = nullptr;
    SDL_GPUDevice *device          = nullptr;
    SDL_GPURenderPass *projectPass = nullptr;
    SDL_GPUTexture *projectTexture = nullptr;
    SDL_GPUSampler *projectSampler = nullptr;
    int width                      = 1280;
    int height                     = 720;
};

struct AppState
{
    ImGuiID mainViewportId = 0;
    float ProjectWindowX   = 0;
    float ProjectWindowY   = 0;
    int projectIndex       = 0;
};

struct Context
{
    Context() = default;
    ~Context();
    void init();

    RenderData renderData {};
    AppState appState {};
    InputHandler inputHandler {};
};

}  // namespace Core

extern Core::Context gContext;
#endif  // INCLUDE_CORE_CONTEXT_H_
