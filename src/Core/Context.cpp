#include "Context.h"

#include "Core/ImGuiCore/ImGuiCore.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Common/SDL_Exception.h"
#include "Utils/Time.h"

Core::Context gContext;

namespace Core
{

void Context::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) { throw SDL_Exception("Couldn't initialize SDL"); }

    renderData.device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL
                                      | SDL_GPU_SHADERFORMAT_MSL,
                                  false, nullptr);

    if (!renderData.device) { throw SDL_Exception("Unable to create SDL_GPUDevice"); }

    renderData.window = SDL_CreateWindow("Ankush's Garage", renderData.width, renderData.height,
                               SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);

    if (!renderData.window) { throw SDL_Exception("Unable to create SDL_Window"); }

    if (!SDL_ClaimWindowForGPUDevice(renderData.device, renderData.window))
    {
        throw SDL_Exception("Unable to claim window for device");
    }

    ImGuiCore::Init();
    Renderer::Init();
    Utils::Time::init();
}

Context::~Context()
{
    ImGuiCore::Quit();
    SDL_ReleaseWindowFromGPUDevice(renderData.device, renderData.window);
}

}  // namespace Core
