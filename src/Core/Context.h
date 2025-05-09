#ifndef INCLUDE_CORE_CONTEXT_H_
#define INCLUDE_CORE_CONTEXT_H_

#include "Common/pch.h"

#include "Core/Event/EventHandler.h"

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
    glm::vec2 ProjectWindowSize = {0.0f, 0.0f};
    glm::vec2 ScrollVal         = {0.0f, 0.0f};
    glm::vec2 MouseRel          = {0.0f, 0.0f};
    ImGuiID MainViewportId      = 0;
    int ProjectIndex            = 0;
    int ProjectToBeIndex        = 0;
    bool projectWindowFocused   = false;
    bool projectWindowHovered   = false;
    bool HasToChangeIndex       = false;
};

class Context
{
  public:
    Context();
    ~Context();
    void init();

    Context(Context &&)                 = delete;
    Context(const Context &)            = delete;
    Context &operator=(Context &&)      = delete;
    Context &operator=(const Context &) = delete;

    static std::shared_ptr<Context> GetContext();

    RenderDataStruct RenderData {};
    AppStateStruct AppState {};
    EventHandlerStruct EventHandler {};

  private:
    // no need of lazy initialisation
    static std::shared_ptr<Context> instance;
};

}  // namespace Core

#endif  // INCLUDE_CORE_CONTEXT_H_
