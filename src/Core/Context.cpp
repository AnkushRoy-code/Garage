#include "Core/Context.h"

#include "Core/ImGuiCore/ImGuiCore.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Common/SDL_Exception.h"
#include "Utils/Time.h"

Core::Context g_Context;

namespace Core
{

void Context::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) { throw SDL_Exception("Couldn't initialize SDL"); }

    RenderData.Device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL
                                      | SDL_GPU_SHADERFORMAT_MSL,
                                  false, nullptr);

    if (!RenderData.Device) { throw SDL_Exception("Unable to create SDL_GPUDevice"); }

    RenderData.Window = SDL_CreateWindow("Ankush's Garage", RenderData.Width, RenderData.Height,
                               SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);

    if (!RenderData.Window) { throw SDL_Exception("Unable to create SDL_Window"); }

    if (!SDL_ClaimWindowForGPUDevice(RenderData.Device, RenderData.Window))
    {
        throw SDL_Exception("Unable to claim window for device");
    }

    ImGuiCore::Init();
    Renderer::Init();
    Utils::Time::Init();
}

Context::~Context()
{
    ImGuiCore::Quit();
    SDL_ReleaseWindowFromGPUDevice(RenderData.Device, RenderData.Window);
}

}  // namespace Core
